#include <gtest/gtest.h>
#include <transport/qt/event/globalQtMessageExchange.h>

#include <QCoreApplication>
#include <thread>
#include <vector>

// Records the payload type of every MessageEvent it receives and, like a real
// consumer, drains the channel the type identifies.
class RecordingReceiver : public QObject
{
public:
  std::vector<PayloadType> received;
  int settingsConsumed = 0;
  int iqConsumed = 0;

  bool event(QEvent* e) override
  {
    if (e->type() < QEvent::User) {
      return QObject::event(e);
    }
    auto payloadType = static_cast<PayloadType>(e->type() - QEvent::User);
    received.push_back(payloadType);

    auto& exchange = globalQtMessageExchange();
    switch (static_cast<int>(payloadType)) {
      case RadioSettings::payloadType:
        exchange.settingsToClient().consumePending([&](RadioSettings&) { ++settingsConsumed; });
        break;
      case PAYLOAD_TYPE_IQ:
        exchange.iqToClient().consumePending([&](IqMessage&) { ++iqConsumed; });
        break;
      default:
        break;
    }
    return true;
  }
};

namespace
{
  template<typename ChannelT>
  bool publish(ChannelT& channel)
  {
    auto* pmsg = channel.reserveWrite();
    if (pmsg == nullptr) {
      return false;
    }
    channel.commitWrite(pmsg);
    return true;
  }

  template<typename ChannelT>
  void drain(ChannelT& channel)
  {
    channel.consumePending([](auto&) {});
  }

  void deliverPostedEvents()
  {
    QCoreApplication::sendPostedEvents();
  }
}

// The exchange is a process-wide singleton, so every test starts and ends with
// nothing attached and every channel empty.
class QtMessageExchangeTest : public ::testing::Test
{
protected:
  QtRadioMessageExchange& exchange = globalQtMessageExchange();

  void SetUp() override { reset(); }
  void TearDown() override { reset(); }

  void reset()
  {
    exchange.detachRadio();
    exchange.detachClient();
    deliverPostedEvents();
    drain(exchange.settingsToRadio());
    drain(exchange.updateToRadio());
    drain(exchange.settingsToClient());
    drain(exchange.iqToClient());
    drain(exchange.modesToClient());
    drain(exchange.bandsToClient());
  }
};

TEST_F(QtMessageExchangeTest, NothingAttachedMessagesWaitInRing)
{
  ASSERT_TRUE(publish(exchange.settingsToClient()));
  deliverPostedEvents();

  EXPECT_EQ(exchange.settingsToClient().pending(), 1u);
}

TEST_F(QtMessageExchangeTest, AttachedClientDrainsMessagesThatArrivedEarlier)
{
  ASSERT_TRUE(publish(exchange.settingsToClient()));

  RecordingReceiver client;
  exchange.attachClient(&client);
  ASSERT_TRUE(publish(exchange.settingsToClient()));
  deliverPostedEvents();

  // Attaching wakes for the early message and the publish wakes again. The
  // first wake drains both; the second finds the channel empty.
  EXPECT_EQ(client.received.size(), 2u);
  EXPECT_EQ(client.settingsConsumed, 2);
}

TEST_F(QtMessageExchangeTest, AttachingWakesClientForRingFilledEarlier)
{
  // A producer that starts first fills the ring, then can commit nothing more,
  // so no further notification will ever come from it.
  int published = 0;
  while (publish(exchange.iqToClient())) {
    ++published;
  }
  ASSERT_GT(published, 0);

  RecordingReceiver client;
  exchange.attachClient(&client);
  deliverPostedEvents();

  const std::vector<PayloadType> expected{PAYLOAD_TYPE_IQ};
  EXPECT_EQ(client.received, expected);
  EXPECT_EQ(client.iqConsumed, published);
  EXPECT_TRUE(publish(exchange.iqToClient()));
}

TEST_F(QtMessageExchangeTest, OneClientTargetRoutesByPayloadType)
{
  RecordingReceiver client;
  exchange.attachClient(&client);

  ASSERT_TRUE(publish(exchange.settingsToClient()));
  ASSERT_TRUE(publish(exchange.iqToClient()));
  ASSERT_TRUE(publish(exchange.iqToClient()));
  deliverPostedEvents();

  const std::vector<PayloadType> expected{
    static_cast<PayloadType>(RadioSettings::payloadType), PAYLOAD_TYPE_IQ, PAYLOAD_TYPE_IQ
  };
  EXPECT_EQ(client.received, expected);
  EXPECT_EQ(client.settingsConsumed, 1);
  // The first IQ wake drains both; the second finds the channel empty.
  EXPECT_EQ(client.iqConsumed, 2);
}

TEST_F(QtMessageExchangeTest, DirectionsAreIndependent)
{
  RecordingReceiver radio;
  RecordingReceiver client;
  exchange.attachRadio(&radio);
  exchange.attachClient(&client);

  ASSERT_TRUE(publish(exchange.settingsToRadio()));
  ASSERT_TRUE(publish(exchange.updateToRadio()));
  deliverPostedEvents();

  const std::vector<PayloadType> expected{
    static_cast<PayloadType>(RadioSettings::payloadType), PAYLOAD_TYPE_FIELD_UPDATE
  };
  EXPECT_EQ(radio.received, expected);
  EXPECT_TRUE(client.received.empty());
}

TEST_F(QtMessageExchangeTest, DetachedClientReceivesNothingFurther)
{
  RecordingReceiver client;
  exchange.attachClient(&client);
  exchange.detachClient();

  ASSERT_TRUE(publish(exchange.settingsToClient()));
  deliverPostedEvents();

  EXPECT_TRUE(client.received.empty());
  EXPECT_EQ(exchange.settingsToClient().pending(), 1u);
}

TEST_F(QtMessageExchangeTest, ClientDestroyedWithUndeliveredEventsIsSafe)
{
  {
    RecordingReceiver client;
    exchange.attachClient(&client);
    ASSERT_TRUE(publish(exchange.settingsToClient()));
    // Destroyed before the posted event is delivered, as a real consumer
    // would be if it detached in its destructor.
    exchange.detachClient();
  }
  deliverPostedEvents();   // must not deliver to the destroyed receiver

  EXPECT_EQ(exchange.settingsToClient().pending(), 1u);
}

// The radio publishes from its own thread; the client drains on this one.
TEST_F(QtMessageExchangeTest, ProducerThreadToAttachedClient)
{
  constexpr int Count = 10000;
  RecordingReceiver client;
  exchange.attachClient(&client);

  std::thread radio([&] {
    for (int i = 0; i < Count; ++i) {
      while (!publish(exchange.iqToClient())) {
        std::this_thread::yield();
      }
    }
  });

  while (client.iqConsumed < Count) {
    deliverPostedEvents();
    std::this_thread::yield();
  }
  radio.join();
  deliverPostedEvents();

  EXPECT_EQ(client.iqConsumed, Count);
  EXPECT_EQ(client.received.size(), static_cast<size_t>(Count));
}

int main(int argc, char** argv)
{
  QCoreApplication app(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
