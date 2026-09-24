#include <gtest/gtest.h>
#include <transport/MessageChannelT.h>

#include <thread>
#include <vector>

// MessageChannelT only needs getPayloadType() from T, so a plain struct stands
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
using RecordingChannel = MessageChannelT<FakeMessage, N, RecordingNotifier>;

namespace
{
  template<typename ChannelT>
  bool publish(ChannelT& channel, PayloadType type, int sequence)
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

TEST(MessageChannelTest, NotifiesOncePerCommitWithMessagePayloadType)
{
  std::vector<PayloadType> calls;
  RecordingChannel<4> channel(RecordingNotifier{&calls});

  ASSERT_TRUE(publish(channel, PAYLOAD_TYPE_SETTINGS_BASIC_RX, 1));
  ASSERT_TRUE(publish(channel, PAYLOAD_TYPE_MODES, 2));
  ASSERT_TRUE(publish(channel, PAYLOAD_TYPE_BANDS, 3));

  const std::vector<PayloadType> expected{
    PAYLOAD_TYPE_SETTINGS_BASIC_RX, PAYLOAD_TYPE_MODES, PAYLOAD_TYPE_BANDS
  };
  EXPECT_EQ(calls, expected);
  EXPECT_EQ(channel.pending(), 3u);
}

TEST(MessageChannelTest, ReserveWithoutCommitDoesNotNotify)
{
  std::vector<PayloadType> calls;
  RecordingChannel<4> channel(RecordingNotifier{&calls});

  ASSERT_NE(channel.reserveWrite(), nullptr);

  EXPECT_TRUE(calls.empty());
  EXPECT_EQ(channel.pending(), 0u);
}

TEST(MessageChannelTest, ConsumePendingDeliversAllInOrder)
{
  std::vector<PayloadType> calls;
  RecordingChannel<4> channel(RecordingNotifier{&calls});

  for (int i = 1; i <= 3; ++i) {
    ASSERT_TRUE(publish(channel, PAYLOAD_TYPE_MODES, i));
  }

  std::vector<int> seen;
  channel.consumePending([&](const FakeMessage& msg) { seen.push_back(msg.sequence); });

  EXPECT_EQ(seen, (std::vector<int>{1, 2, 3}));
  EXPECT_EQ(channel.pending(), 0u);
}

TEST(MessageChannelTest, ConsumePendingOnEmptyChannelIsHarmless)
{
  std::vector<PayloadType> calls;
  RecordingChannel<4> channel(RecordingNotifier{&calls});

  int handled = 0;
  channel.consumePending([&](const FakeMessage&) { ++handled; });

  EXPECT_EQ(handled, 0);
}

TEST(MessageChannelTest, FullChannelRefusesUntilConsumed)
{
  std::vector<PayloadType> calls;
  RecordingChannel<2> channel(RecordingNotifier{&calls});

  ASSERT_TRUE(publish(channel, PAYLOAD_TYPE_MODES, 1));
  ASSERT_TRUE(publish(channel, PAYLOAD_TYPE_MODES, 2));
  EXPECT_FALSE(publish(channel, PAYLOAD_TYPE_MODES, 3));
  EXPECT_EQ(calls.size(), 2u);

  channel.consumePending([](const FakeMessage&) {});

  // Slots are returned to circulation, so the channel accepts writes again.
  EXPECT_TRUE(publish(channel, PAYLOAD_TYPE_MODES, 4));
  EXPECT_TRUE(publish(channel, PAYLOAD_TYPE_MODES, 5));
}

TEST(MessageChannelTest, SlotsReturnedEvenWhenHandlerIgnoresMessage)
{
  std::vector<PayloadType> calls;
  RecordingChannel<2> channel(RecordingNotifier{&calls});

  // Many more messages than slots: only works if every slot is recycled.
  for (int i = 0; i < 10; ++i) {
    ASSERT_TRUE(publish(channel, PAYLOAD_TYPE_MODES, i));
    channel.consumePending([](const FakeMessage&) {});
  }
  EXPECT_EQ(calls.size(), 10u);
}

// One producer thread, one consumer thread, a small ring: every message must
// arrive exactly once and in order, with one notification per commit.
TEST(MessageChannelTest, ProducerAndConsumerThreads)
{
  constexpr int Count = 100000;
  int notifications = 0;
  MessageChannelT<FakeMessage, 4, CountingNotifier> channel(CountingNotifier{&notifications});

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
