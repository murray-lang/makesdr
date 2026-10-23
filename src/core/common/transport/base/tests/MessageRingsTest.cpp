#include <gtest/gtest.h>
#include <transport/MessageRingT.h>

#include <thread>
#include <vector>

// MessagePipeT only needs getPayloadType() from T, so a plain struct stands
// in for a real message and keeps this test free of settings models.
struct FakeMessage
{
  PayloadType type = PAYLOAD_TYPE_NONE;
  int         sequence = 0;
  PayloadType getPayloadType() const { return type; }
};

// Records every payload type it is called with, in order.
struct RecordingNotifier
{
  std::vector<PayloadType>* calls;
  void operator()(PayloadType payloadType) const { calls->push_back(payloadType); }
};

// Counts calls only; safe to use from the producer thread alone.
struct CountingNotifier
{
  int* count;
  void operator()(PayloadType) const { ++*count; }
};

template<uint8_t N>
using RecordingPipe = MessageRingT<FakeMessage, N, RecordingNotifier>;

namespace
{
  template<typename PipeT>
  bool publish(PipeT& channel, PayloadType type, int sequence)
  {
    FakeMessage* pmsg = channel.reserveWrite();
    if (pmsg == nullptr) {
      return false;
    }
    pmsg->type = type;
    pmsg->sequence = sequence;
    channel.commitWrite(pmsg);
    return true;
  }
}

TEST(MessagePipeTest, NotifiesOncePerCommitWithMessagePayloadType)
{
  std::vector<PayloadType> calls;
  RecordingPipe<4> channel(RecordingNotifier{&calls});

  ASSERT_TRUE(publish(channel, PAYLOAD_TYPE_SETTINGS_BASIC_RX, 1));
  ASSERT_TRUE(publish(channel, PAYLOAD_TYPE_MODES, 2));
  ASSERT_TRUE(publish(channel, PAYLOAD_TYPE_BANDS, 3));

  const std::vector<PayloadType> expected{
    PAYLOAD_TYPE_SETTINGS_BASIC_RX, PAYLOAD_TYPE_MODES, PAYLOAD_TYPE_BANDS
  };
  EXPECT_EQ(calls, expected);
  EXPECT_EQ(channel.pending(), 3u);
}

TEST(MessagePipeTest, ReserveWithoutCommitDoesNotNotify)
{
  std::vector<PayloadType> calls;
  RecordingPipe<4> channel(RecordingNotifier{&calls});

  ASSERT_NE(channel.reserveWrite(), nullptr);

  EXPECT_TRUE(calls.empty());
  EXPECT_EQ(channel.pending(), 0u);
}

TEST(MessagePipeTest, ConsumePendingDeliversAllInOrder)
{
  std::vector<PayloadType> calls;
  RecordingPipe<4> channel(RecordingNotifier{&calls});

  for (int i = 1; i <= 3; ++i) {
    ASSERT_TRUE(publish(channel, PAYLOAD_TYPE_MODES, i));
  }

  std::vector<int> seen;
  channel.consumePending([&](const FakeMessage& msg) { seen.push_back(msg.sequence); });

  EXPECT_EQ(seen, (std::vector<int>{1, 2, 3}));
  EXPECT_EQ(channel.pending(), 0u);
}

TEST(MessagePipeTest, ConsumePendingOnEmptyPipeIsHarmless)
{
  std::vector<PayloadType> calls;
  RecordingPipe<4> channel(RecordingNotifier{&calls});

  int handled = 0;
  channel.consumePending([&](const FakeMessage&) { ++handled; });

  EXPECT_EQ(handled, 0);
}

TEST(MessagePipeTest, FullPipeRefusesUntilConsumed)
{
  std::vector<PayloadType> calls;
  RecordingPipe<2> channel(RecordingNotifier{&calls});

  ASSERT_TRUE(publish(channel, PAYLOAD_TYPE_MODES, 1));
  ASSERT_TRUE(publish(channel, PAYLOAD_TYPE_MODES, 2));
  EXPECT_FALSE(publish(channel, PAYLOAD_TYPE_MODES, 3));
  EXPECT_EQ(calls.size(), 2u);

  channel.consumePending([](const FakeMessage&) {});

  // Slots are returned to circulation, so the channel accepts writes again.
  EXPECT_TRUE(publish(channel, PAYLOAD_TYPE_MODES, 4));
  EXPECT_TRUE(publish(channel, PAYLOAD_TYPE_MODES, 5));
}

TEST(MessagePipeTest, SlotsReturnedEvenWhenHandlerIgnoresMessage)
{
  std::vector<PayloadType> calls;
  RecordingPipe<2> channel(RecordingNotifier{&calls});

  // Many more messages than slots: only works if every slot is recycled.
  for (int i = 0; i < 10; ++i) {
    ASSERT_TRUE(publish(channel, PAYLOAD_TYPE_MODES, i));
    channel.consumePending([](const FakeMessage&) {});
  }
  EXPECT_EQ(calls.size(), 10u);
}

// One producer thread, one consumer thread, a small ring: every message must
// arrive exactly once and in order, with one notification per commit.
TEST(MessagePipeTest, ProducerAndConsumerThreads)
{
  constexpr int Count = 100000;
  int notifications = 0;
  MessageRingT<FakeMessage, 4, CountingNotifier> channel(CountingNotifier{&notifications});

  std::thread producer([&] {
    for (int i = 0; i < Count; ++i) {
      while (!publish(channel, PAYLOAD_TYPE_MODES, i)) {
        std::this_thread::yield();
      }
    }
  });

  int expected = 0;
  bool inOrder = true;
  while (expected < Count) {
    channel.consumePending([&](const FakeMessage& msg) {
      inOrder = inOrder && (msg.sequence == expected);
      ++expected;
    });
    std::this_thread::yield();
  }

  producer.join();

  EXPECT_TRUE(inOrder);
  EXPECT_EQ(expected, Count);
  EXPECT_EQ(notifications, Count);
  EXPECT_EQ(channel.pending(), 0u);
}

//--- attaching the recipient -------------------------------------------------

// Waking on attach needs the ring's payload type without a message to ask.
struct ModesMessage : FakeMessage
{
  static constexpr int payloadType = PAYLOAD_TYPE_MODES;
};

// Records the payload types it is called with, but only while attached.
struct AttachableNotifier
{
  std::vector<PayloadType>* target = nullptr;

  void attach(std::vector<PayloadType>* t) { target = t; }
  void detach()                            { target = nullptr; }
  void operator()(PayloadType payloadType) const
  {
    if (target) target->push_back(payloadType);
  }
};

using AttachableRing = MessageRingT<ModesMessage, 2, AttachableNotifier>;

TEST(MessagePipeTest, NothingAttachedMessagesWaitInRing)
{
  AttachableRing ring;
  ModesMessage* pmsg = ring.reserveWrite();
  ring.commitWrite(pmsg);
  EXPECT_EQ(ring.pending(), 1u);
}

TEST(MessagePipeTest, AttachingRecipientWakesForMessagesCommittedEarlier)
{
  AttachableRing ring;
  // Fill the ring, so the producer could commit nothing more to wake anyone.
  ring.commitWrite(ring.reserveWrite());
  ring.commitWrite(ring.reserveWrite());
  ASSERT_EQ(ring.reserveWrite(), nullptr);

  std::vector<PayloadType> calls;
  ring.attachRecipient(&calls);
  EXPECT_EQ(calls, std::vector<PayloadType>{PAYLOAD_TYPE_MODES});
}

TEST(MessagePipeTest, AttachingRecipientToEmptyRingDoesNotWake)
{
  AttachableRing ring;
  std::vector<PayloadType> calls;
  ring.attachRecipient(&calls);
  EXPECT_TRUE(calls.empty());
}

TEST(MessagePipeTest, DetachedRecipientIsNoLongerWoken)
{
  AttachableRing ring;
  std::vector<PayloadType> calls;
  ring.attachRecipient(&calls);
  ring.detachRecipient();
  ring.commitWrite(ring.reserveWrite());
  EXPECT_TRUE(calls.empty());
}
