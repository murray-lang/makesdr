#include <gtest/gtest.h>
#include <transport/MessageChannelT.h>
#include <transport/MessageChannelSinkT.h>
#include <transport/MessageChannelSourceT.h>

#include <vector>

namespace
{
  struct Message
  {
    int sequence = 0;
    PayloadType getPayloadType() const { return PAYLOAD_TYPE_MODES; }
  };

  struct CountingNotifier
  {
    int count = 0;
    void operator()(PayloadType) { ++count; }
  };

  using Channel = MessageChannelT<Message, 2, CountingNotifier>;

  // Stands in for whatever the source delivers to: records what it was given.
  struct RecordingSink : public MessageSinkT<Message>
  {
    std::vector<int> sequences;
    ResultCode applyMessage(Message* message) override
    {
      sequences.push_back(message->sequence);
      return ResultCode::OK;
    }
  };
}

TEST(MessageChannelSinkTest, CopiesMessageIntoChannelAndNotifies)
{
  Channel channel;
  MessageChannelSinkT<Channel> sink(channel);

  Message message{.sequence = 7};
  EXPECT_EQ(sink.applyMessage(&message), ResultCode::OK);

  // The caller's message is copied, so changing it afterwards has no effect.
  message.sequence = 99;

  std::vector<int> seen;
  channel.consumePending([&](Message& m) { seen.push_back(m.sequence); });
  EXPECT_EQ(seen, (std::vector<int>{7}));
  EXPECT_EQ(channel.notifier().count, 1);
}

TEST(MessageChannelSinkTest, FullChannelDropsAndReportsWithoutNotifying)
{
  Channel channel;
  MessageChannelSinkT<Channel> sink(channel);
  Message message;

  EXPECT_EQ(sink.applyMessage(&message), ResultCode::OK);
  EXPECT_EQ(sink.applyMessage(&message), ResultCode::OK);
  EXPECT_EQ(sink.applyMessage(&message), ResultCode::ERR_CHANNEL_FULL);

  EXPECT_EQ(channel.notifier().count, 2);
  EXPECT_EQ(channel.pending(), 2u);
}

TEST(MessageChannelSourceTest, DeliversPendingMessagesToSinkInOrder)
{
  Channel channel;
  MessageChannelSinkT<Channel> producer(channel);
  MessageChannelSourceT<Channel> source(channel);
  RecordingSink consumer;
  source.connectMessageSink(&consumer);

  Message a{.sequence = 1};
  Message b{.sequence = 2};
  producer.applyMessage(&a);
  producer.applyMessage(&b);
  source.consumePending();

  EXPECT_EQ(consumer.sequences, (std::vector<int>{1, 2}));
  EXPECT_EQ(channel.pending(), 0u);
}

TEST(MessageChannelSourceTest, WithNoSinkMessagesAreDiscardedAndSlotsRecycled)
{
  Channel channel;
  MessageChannelSinkT<Channel> producer(channel);
  MessageChannelSourceT<Channel> source(channel);
  Message message;

  // Many more messages than slots: only works if every slot comes back.
  for (int i = 0; i < 10; ++i) {
    ASSERT_EQ(producer.applyMessage(&message), ResultCode::OK);
    source.consumePending();
  }
  EXPECT_EQ(channel.pending(), 0u);
}

TEST(MessageChannelSourceTest, UsableThroughConsumerInterface)
{
  Channel channel;
  MessageChannelSinkT<Channel> producer(channel);
  MessageChannelSourceT<Channel> source(channel);
  RecordingSink consumer;
  source.connectMessageSink(&consumer);

  Message message{.sequence = 3};
  producer.applyMessage(&message);

  // What a wake-up receiver holds: it need not know the message type.
  IMessageChannelConsumer& woken = source;
  woken.consumePending();

  EXPECT_EQ(consumer.sequences, (std::vector<int>{3}));
}
