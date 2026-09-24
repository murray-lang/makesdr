#include <gtest/gtest.h>
#include <transport/radio/QtRadioTransportT.h>

#include <QCoreApplication>
#include <atomic>
#include <chrono>
#include <thread>

using TestQtRadioTransport = QtRadioTransportT<RadioSettings>;

namespace
{
  // Stands in for the radio: counts what arrives and notes which thread
  // delivered it.
  template<typename MessageT>
  struct RecordingSink : public MessageSinkT<MessageT>
  {
    std::atomic<int>             applied{0};
    std::atomic<std::thread::id> threadId{};

    ResultCode applyMessage(MessageT*) override
    {
      threadId = std::this_thread::get_id();
      ++applied;
      return ResultCode::OK;
    }
  };

  template<typename ChannelT>
  void publish(ChannelT& channel)
  {
    auto* pmsg = channel.reserveWrite();
    ASSERT_NE(pmsg, nullptr);
    channel.commitWrite(pmsg);
  }

  template<typename ChannelT>
  void drain(ChannelT& channel)
  {
    channel.consumePending([](auto&) {});
  }

  bool waitFor(const std::atomic<int>& counter, int target)
  {
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    while (counter < target) {
      if (std::chrono::steady_clock::now() > deadline) {
        return false;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return true;
  }
}

class QtRadioTransportTest : public ::testing::Test
{
protected:
  QtRadioMessageExchange& exchange = globalQtMessageExchange();

  void TearDown() override
  {
    drain(exchange.settingsToRadio());
    drain(exchange.updateToRadio());
    drain(exchange.settingsToClient());
    drain(exchange.iqToClient());
    drain(exchange.modesToClient());
    drain(exchange.bandsToClient());
  }
};

TEST_F(QtRadioTransportTest, SendGoesIntoClientBoundChannels)
{
  TestQtRadioTransport transport;

  RadioSettings settings;
  ModeList modes;
  BandCategoryList bands;
  EXPECT_EQ(transport.send(&settings), ResultCode::OK);
  EXPECT_EQ(transport.send(&modes), ResultCode::OK);
  EXPECT_EQ(transport.send(&bands), ResultCode::OK);

  EXPECT_EQ(exchange.settingsToClient().pending(), 1u);
  EXPECT_EQ(exchange.modesToClient().pending(), 1u);
  EXPECT_EQ(exchange.bandsToClient().pending(), 1u);
  EXPECT_EQ(exchange.settingsToRadio().pending(), 0u);
}

TEST_F(QtRadioTransportTest, IncomingMessagesReachSinksOnRadioThread)
{
  TestQtRadioTransport transport;
  RecordingSink<RadioSettings> settingsSink;
  RecordingSink<FieldUpdateMessage> updateSink;
  transport.connectRadioSettingsSink(&settingsSink);
  transport.connectFieldUpdateSink(&updateSink);
  ASSERT_EQ(transport.start(), ResultCode::OK);

  // Published from this thread, as the client would.
  publish(exchange.settingsToRadio());
  publish(exchange.updateToRadio());
  publish(exchange.updateToRadio());

  EXPECT_TRUE(waitFor(settingsSink.applied, 1));
  EXPECT_TRUE(waitFor(updateSink.applied, 2));
  EXPECT_NE(settingsSink.threadId.load(), std::this_thread::get_id());
  EXPECT_NE(updateSink.threadId.load(), std::this_thread::get_id());

  transport.stop();
}

TEST_F(QtRadioTransportTest, AfterStopIncomingMessagesWait)
{
  TestQtRadioTransport transport;
  RecordingSink<RadioSettings> settingsSink;
  transport.connectRadioSettingsSink(&settingsSink);
  ASSERT_EQ(transport.start(), ResultCode::OK);
  transport.stop();

  publish(exchange.settingsToRadio());
  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  EXPECT_EQ(settingsSink.applied, 0);
  EXPECT_EQ(exchange.settingsToRadio().pending(), 1u);
}

TEST_F(QtRadioTransportTest, RestartAfterStopDeliversAgain)
{
  TestQtRadioTransport transport;
  RecordingSink<RadioSettings> settingsSink;
  transport.connectRadioSettingsSink(&settingsSink);
  ASSERT_EQ(transport.start(), ResultCode::OK);
  transport.stop();
  ASSERT_EQ(transport.start(), ResultCode::OK);

  publish(exchange.settingsToRadio());

  EXPECT_TRUE(waitFor(settingsSink.applied, 1));
  transport.stop();
}

int main(int argc, char** argv)
{
  QCoreApplication app(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
