#include <gtest/gtest.h>
#include <transport/MessageRingT.h>
#include <transport/MessageRingSinkT.h>
#include <transport/MessageRingSourceT.h>

#include <type_traits>
#include <vector>

// How a realistic message survives a trip through a ring, which carries only
// its payload. The fake message reproduces what makes real messages more
// than their payload:
//   - a vtable, like every MessageT (via IMessage);
//   - a view over its own payload, like ModeList and BandCategoryList, which a
//     raw payload copy would leave stale;
//   - an operator= with semantics beyond copying, like the settings classes,
//     whose replace(body, true) marks every field present.

namespace
{
  constexpr int MaxValues = 8;

  struct FakeHeader
  {
    int payloadType;
    int source;
  };

  // Stands in for a nanopb payload struct: plain, fixed size, no pointers.
  struct FakePayload
  {
    FakeHeader header;
    bool       allPresent;
    int        count;
    int        values[MaxValues];
  };

  struct View
  {
    const int* data = nullptr;
    int        size = 0;
  };

  class FakeMessage
  {
  public:
    using PayloadProto = FakePayload;
    static const int payloadType = PAYLOAD_TYPE_BANDS;

    FakeMessage() : m_payload{}, m_view{m_payload.values, 0} {}

    explicit FakeMessage(const FakePayload& payload)
      : m_payload(payload)
      , m_view{m_payload.values, payload.count}
    {
    }

    FakeMessage(const FakeMessage& other) : FakeMessage(other.m_payload) {}

    // Like the settings classes: the body is replaced and every field marked
    // present, while the header is left for the sink to copy.
    FakeMessage& operator=(const FakeMessage& other)
    {
      if (this != &other) {
        m_payload.allPresent = true;
        m_payload.count = other.m_payload.count;
        for (int i = 0; i < MaxValues; ++i) {
          m_payload.values[i] = other.m_payload.values[i];
        }
        m_view = View{m_payload.values, m_payload.count};
      }
      return *this;
    }

    virtual ~FakeMessage() = default;

    // Never called by the ring: slots hold no object to call it on.
    virtual PayloadType getPayloadType() { return static_cast<PayloadType>(payloadType); }

    void set(int source, std::initializer_list<int> values)
    {
      m_payload.header.payloadType = payloadType;
      m_payload.header.source = source;
      m_payload.allPresent = false;
      m_payload.count = 0;
      for (int value : values) {
        m_payload.values[m_payload.count++] = value;
      }
      m_view = View{m_payload.values, m_payload.count};
    }

    FakePayload& payload() { return m_payload; }
    [[nodiscard]] const FakePayload& payload() const { return m_payload; }
    [[nodiscard]] const View& view() const { return m_view; }

  private:
    FakePayload m_payload;
    View        m_view;
  };

  struct PayloadNotifier
  {
    std::vector<PayloadType> calls;
    void operator()(PayloadType payloadType) { calls.push_back(payloadType); }
  };

  using Ring = MessageRingT<FakeMessage, 2, PayloadNotifier>;
  using Sink = MessageRingSinkT<Ring>;
  using Source = MessageRingSourceT<Ring>;

  // What the consumer was given, captured while the pointer is valid.
  struct Received
  {
    int              source;
    bool             allPresent;
    std::vector<int> values;
    bool             viewMatchesCount;
    bool             viewIsIntoMessage;
    const void*      address;
  };

  struct RecordingSink : public MessageSinkT<FakeMessage>
  {
    std::vector<Received> received;
    ResultCode applyMessage(FakeMessage* message) override
    {
      const FakePayload& payload = message->payload();
      const View& view = message->view();
      received.push_back(Received{
        payload.header.source,
        payload.allPresent,
        std::vector<int>(view.data, view.data + view.size),
        view.size == payload.count,
        view.data == payload.values,
        message
      });
      return ResultCode::OK;
    }
  };
}

TEST(MessageRingMessageSemanticsTest, SlotsHoldOnlyThePayload)
{
  static_assert(std::is_same_v<Ring::SlotType, FakePayload>);
  static_assert(std::is_trivially_copyable_v<Ring::SlotType>);
}

TEST(MessageRingMessageSemanticsTest, DeliversMessageRebuiltFromPayload)
{
  Ring ring;
  Sink sink(ring);
  Source source(ring);
  RecordingSink recorder;
  source.connectMessageSink(&recorder);

  FakeMessage message;
  message.set(7, {10, 20, 30});
  ASSERT_EQ(sink.applyMessage(&message), ResultCode::OK);
  source.consumePending();

  ASSERT_EQ(recorder.received.size(), 1u);
  const Received& got = recorder.received[0];
  EXPECT_EQ(got.values, (std::vector<int>{10, 20, 30}));
  EXPECT_TRUE(got.viewMatchesCount);
  EXPECT_TRUE(got.viewIsIntoMessage);
  EXPECT_NE(got.address, &message);
}

TEST(MessageRingMessageSemanticsTest, AppliesMessageAssignmentAndCopiesHeader)
{
  Ring ring;
  Sink sink(ring);
  Source source(ring);
  RecordingSink recorder;
  source.connectMessageSink(&recorder);

  FakeMessage message;
  message.set(42, {1});
  ASSERT_FALSE(message.payload().allPresent);

  ASSERT_EQ(sink.applyMessage(&message), ResultCode::OK);
  source.consumePending();

  // Same result as a ring storing whole messages: operator= has marked every
  // field present, and the header travelled too.
  ASSERT_EQ(recorder.received.size(), 1u);
  EXPECT_TRUE(recorder.received[0].allPresent);
  EXPECT_EQ(recorder.received[0].source, 42);
}

TEST(MessageRingMessageSemanticsTest, ViewFollowsEachPayloadsCount)
{
  Ring ring;
  Sink sink(ring);
  Source source(ring);
  RecordingSink recorder;
  source.connectMessageSink(&recorder);

  // A longer message then a shorter one through the same local message: the
  // second must not be seen through the first's view.
  FakeMessage message;
  message.set(1, {1, 2, 3, 4, 5});
  ASSERT_EQ(sink.applyMessage(&message), ResultCode::OK);
  message.set(2, {9});
  ASSERT_EQ(sink.applyMessage(&message), ResultCode::OK);
  source.consumePending();

  ASSERT_EQ(recorder.received.size(), 2u);
  EXPECT_EQ(recorder.received[0].values, (std::vector<int>{1, 2, 3, 4, 5}));
  EXPECT_EQ(recorder.received[1].values, (std::vector<int>{9}));
  EXPECT_TRUE(recorder.received[1].viewMatchesCount);
}

TEST(MessageRingMessageSemanticsTest, NotifiesWithStaticPayloadType)
{
  Ring ring;
  Sink sink(ring);

  FakeMessage message;
  message.set(0, {1});
  ASSERT_EQ(sink.applyMessage(&message), ResultCode::OK);

  ASSERT_EQ(ring.notifier().calls.size(), 1u);
  EXPECT_EQ(ring.notifier().calls[0], PAYLOAD_TYPE_BANDS);
}

TEST(MessageRingMessageSemanticsTest, PublishesInPlace)
{
  Ring ring;
  Sink sink(ring);
  Source source(ring);
  RecordingSink recorder;
  source.connectMessageSink(&recorder);

  IMessagePublisherT<FakeMessage>& publisher = sink;
  FakeMessage* pmsg = publisher.reserve();
  ASSERT_NE(pmsg, nullptr);
  EXPECT_EQ(publisher.reserve(), pmsg);       // idempotent until commit
  pmsg->set(3, {4, 5});
  publisher.commit(pmsg);
  source.consumePending();

  ASSERT_EQ(recorder.received.size(), 1u);
  EXPECT_EQ(recorder.received[0].source, 3);
  EXPECT_EQ(recorder.received[0].values, (std::vector<int>{4, 5}));
  EXPECT_TRUE(recorder.received[0].viewMatchesCount);
}

TEST(MessageRingMessageSemanticsTest, FullRingRefusesWithoutBlocking)
{
  Ring ring;
  Sink sink(ring);
  Source source(ring);
  RecordingSink recorder;
  source.connectMessageSink(&recorder);

  FakeMessage message;
  message.set(0, {1});
  ASSERT_EQ(sink.applyMessage(&message), ResultCode::OK);
  ASSERT_EQ(sink.applyMessage(&message), ResultCode::OK);

  EXPECT_EQ(sink.applyMessage(&message), ResultCode::ERR_CHANNEL_FULL);
  EXPECT_EQ(sink.reserve(), nullptr);

  source.consumePending();
  EXPECT_EQ(recorder.received.size(), 2u);
  EXPECT_NE(sink.reserve(), nullptr);         // slots back in circulation
}

TEST(MessageRingMessageSemanticsTest, KeepsOrderAcrossManyCycles)
{
  Ring ring;
  Sink sink(ring);
  Source source(ring);
  RecordingSink recorder;
  source.connectMessageSink(&recorder);

  FakeMessage message;
  std::vector<int> expected;
  for (int i = 0; i < 20; ++i) {
    message.set(i, {i, i * 2});
    ASSERT_EQ(sink.applyMessage(&message), ResultCode::OK);
    expected.push_back(i);
    if (i % 2 == 1) {
      source.consumePending();
    }
  }

  ASSERT_EQ(recorder.received.size(), expected.size());
  for (size_t i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(recorder.received[i].source, expected[i]);
    EXPECT_EQ(recorder.received[i].values, (std::vector<int>{expected[i], expected[i] * 2}));
  }
}

TEST(MessageRingMessageSemanticsTest, DiscardsWithNoSinkConnected)
{
  Ring ring;
  Sink sink(ring);
  Source source(ring);

  FakeMessage message;
  message.set(0, {1});
  ASSERT_EQ(sink.applyMessage(&message), ResultCode::OK);
  ASSERT_EQ(sink.applyMessage(&message), ResultCode::OK);
  source.consumePending();

  EXPECT_EQ(ring.pending(), 0u);
  EXPECT_EQ(sink.applyMessage(&message), ResultCode::OK);
}
