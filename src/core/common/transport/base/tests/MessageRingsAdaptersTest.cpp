#include <gtest/gtest.h>
#include <transport/MessageRingT.h>
#include <transport/MessageRingSinkT.h>
#include <transport/MessageRingSourceT.h>

#include <vector>

namespace
{
  struct Payload
  {
    int sequence = 0;
  };

  // The minimum a message needs to travel through a ring: a payload struct,
  // a fixed payload type, and construction from its payload.
  struct Message
  {
    using PayloadProto = Payload;
    static constexpr int payloadType = PAYLOAD_TYPE_MODES;

    Message() = default;
    explicit Message(int sequence) : m_payload{sequence} {}
    explicit Message(const Payload& payload) : m_payload(payload) {}

    Payload& payload() { return m_payload; }
    [[nodiscard]] const Payload& payload() const { return m_payload; }
    [[nodiscard]] int sequence() const { return m_payload.sequence; }

    Payload m_payload;
  };

  struct CountingNotifier
  {
    int count = 0;
    void operator()(PayloadType) { ++count; }
  };

  using Ring = MessageRingT<Message, 2, CountingNotifier>;

  // Stands in for whatever the source delivers to: records what it was given.
  struct RecordingSink : public MessageSinkT<Message>
  {
    std::vector<int> sequences;
    ResultCode applyMessage(Message* message) override
    {
      sequences.push_back(message->sequence());
      return ResultCode::OK;
    }
  };
}

TEST(MessagePipeSinkTest, CopiesMessageIntoPipeAndNotifies)
{
  Ring channel;
  MessageRingSinkT<Ring> sink(channel);

  Message message(7);
  EXPECT_EQ(sink.applyMessage(&message), ResultCode::OK);

  // The caller's message is copied, so changing it afterwards has no effect.
  message.payload().sequence = 99;

  std::vector<int> seen;
  channel.consumePending([&](const Payload& p) { seen.push_back(p.sequence); });
  EXPECT_EQ(seen, (std::vector<int>{7}));
  EXPECT_EQ(channel.notifier().count, 1);
}

TEST(MessagePipeSinkTest, FullPipeDropsAndReportsWithoutNotifying)
{
  Ring channel;
  MessageRingSinkT<Ring> sink(channel);
  Message message;

  EXPECT_EQ(sink.applyMessage(&message), ResultCode::OK);
  EXPECT_EQ(sink.applyMessage(&message), ResultCode::OK);
  EXPECT_EQ(sink.applyMessage(&message), ResultCode::ERR_CHANNEL_FULL);

  EXPECT_EQ(channel.notifier().count, 2);
  EXPECT_EQ(channel.pending(), 2u);
}

TEST(MessagePipeSourceTest, DeliversPendingMessagesToSinkInOrder)
{
  Ring channel;
  MessageRingSinkT<Ring> producer(channel);
  MessageRingSourceT<Ring> source(channel);
  RecordingSink consumer;
  source.connectMessageSink(&consumer);

  Message a(1);
  Message b(2);
  producer.applyMessage(&a);
  producer.applyMessage(&b);
  source.consumePending();

  EXPECT_EQ(consumer.sequences, (std::vector<int>{1, 2}));
  EXPECT_EQ(channel.pending(), 0u);
}

TEST(MessagePipeSourceTest, WithNoSinkMessagesAreDiscardedAndSlotsRecycled)
{
  Ring channel;
  MessageRingSinkT<Ring> producer(channel);
  MessageRingSourceT<Ring> source(channel);
  Message message;

  // Many more messages than slots: only works if every slot comes back.
  for (int i = 0; i < 10; ++i) {
    ASSERT_EQ(producer.applyMessage(&message), ResultCode::OK);
    source.consumePending();
  }
  EXPECT_EQ(channel.pending(), 0u);
}

TEST(MessagePipeSourceTest, UsableThroughConsumerInterface)
{
  Ring channel;
  MessageRingSinkT<Ring> producer(channel);
  MessageRingSourceT<Ring> source(channel);
  RecordingSink consumer;
  source.connectMessageSink(&consumer);

  Message message(3);
  producer.applyMessage(&message);

  // What a wake-up receiver holds: it need not know the message type.
  IMessageRingConsumer& woken = source;
  woken.consumePending();

  EXPECT_EQ(consumer.sequences, (std::vector<int>{3}));
}
