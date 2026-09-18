#include <gtest/gtest.h>
#include <config/struct/QtTransportConfig.h>
#include <linux/test-utils/QtTransportInOutT.h>
#include <settings/model/message/FieldUpdateMessage.h>
#include <settings/model/radios/iq/SplitBandDualIqRxTxSettings.h>
#include <settings/model/data/mode/ModeList.h>
#include <settings/model/data/band/BandCategoryList.h>

using Target = Config::QtTransport::Target;
using Message = Config::QtTransport::Message;

struct TransportIoConfig
{
  Config::QtTransportIn::Fields in;
  Config::QtTransportOut::Fields out;
};


TEST(QtTransportTest, CanTransportSettingsRadioToClient) {

  TransportIoConfig radioConfig  {
    .in =  { .message = Message::Settings, .target = Target::Radio },
    .out = { .message = Message::Settings, .target = Target::Client }
  };
  TransportIoConfig clientConfig {
    .in = { .message = Message::Settings, .target = Target::Client },
    .out = { .message = Message::Settings, .target = Target::Radio }
  };

  QThread thread;

  QtTransportInOutT<SplitBandDualIqRxTxSettings> radio(thread);
  ASSERT_EQ(radio.configure(radioConfig.in, radioConfig.out), ResultCode::OK);

  QtTransportInOutT<SplitBandDualIqRxTxSettings> client(thread);
  ASSERT_EQ(client.configure(clientConfig.in, clientConfig.out), ResultCode::OK);

  ASSERT_EQ(radio.start(), ResultCode::OK);
  ASSERT_EQ(client.start(), ResultCode::OK);

  SplitBandDualIqRxTxSettings settings;
  ASSERT_EQ(radio.send(&settings), ResultCode::OK);

  ASSERT_TRUE(client.waitForMessage(std::chrono::milliseconds(2000)));

  radio.stop();
  client.stop();

  EXPECT_TRUE(client.messageReceived());
}


TEST(QtTransportTest, CanTransportUpdateClientToRadio) {
  TransportIoConfig radioConfig  {
    .in =  { .message = Message::Update, .target = Target::Radio },
    .out = { .message = Message::None, .target = Target::Client }
  };
  TransportIoConfig clientConfig {
    .in = { .message = Message::None, .target = Target::Client },
    .out = { .message = Message::Update, .target = Target::Radio }
  };

  QThread thread;

  QtTransportInOutT<FieldUpdateMessage> radio(thread);
  ASSERT_EQ(radio.configure(radioConfig.in, radioConfig.out), ResultCode::OK);

  QtTransportInOutT<FieldUpdateMessage> client(thread);
  ASSERT_EQ(client.configure(clientConfig.in, clientConfig.out), ResultCode::OK);

  ASSERT_EQ(radio.start(), ResultCode::OK);
  ASSERT_EQ(client.start(), ResultCode::OK);

  FieldUpdateMessage update;
  ASSERT_EQ(client.send(&update), ResultCode::OK);

  ASSERT_TRUE(radio.waitForMessage(std::chrono::milliseconds(2000)));

  radio.stop();
  client.stop();

  EXPECT_TRUE(radio.messageReceived());
}

TEST(QtTransportTest, CanTransportModesRadioToClient) {
  TransportIoConfig radioConfig  {
    .in =  { .message = Message::None, .target = Target::Radio },
    .out = { .message = Message::Modes, .target = Target::Client }
  };
  TransportIoConfig clientConfig {
    .in = { .message = Message::Modes, .target = Target::Client },
    .out = { .message = Message::None, .target = Target::Radio }
  };

  QThread thread;

  QtTransportInOutT<ModeList> radio(thread);
  ASSERT_EQ(radio.configure(radioConfig.in, radioConfig.out), ResultCode::OK);

  QtTransportInOutT<ModeList> client(thread);
  ASSERT_EQ(client.configure(clientConfig.in, clientConfig.out), ResultCode::OK);

  ASSERT_EQ(radio.start(), ResultCode::OK);
  ASSERT_EQ(client.start(), ResultCode::OK);

  ModeList modes;
  ASSERT_EQ(radio.send(&modes), ResultCode::OK);

  ASSERT_TRUE(client.waitForMessage(std::chrono::milliseconds(2000)));

  radio.stop();
  client.stop();

  EXPECT_TRUE(client.messageReceived());
}

TEST(QtTransportTest, CanTransportBandsRadioToClient) {
  TransportIoConfig radioConfig  {
    .in =  { .message = Message::None, .target = Target::Radio },
    .out = { .message = Message::Bands, .target = Target::Client }
  };
  TransportIoConfig clientConfig {
    .in = { .message = Message::Bands, .target = Target::Client },
    .out = { .message = Message::None, .target = Target::Radio }
  };

  QThread thread;

  QtTransportInOutT<BandCategoryList> radio(thread);
  ASSERT_EQ(radio.configure(radioConfig.in, radioConfig.out), ResultCode::OK);

  QtTransportInOutT<BandCategoryList> client(thread);
  ASSERT_EQ(client.configure(clientConfig.in, clientConfig.out), ResultCode::OK);

  ASSERT_EQ(radio.start(), ResultCode::OK);
  ASSERT_EQ(client.start(), ResultCode::OK);

  BandCategoryList bands;
  ASSERT_EQ(radio.send(&bands), ResultCode::OK);

  ASSERT_TRUE(client.waitForMessage(std::chrono::milliseconds(2000)));

  radio.stop();
  client.stop();

  EXPECT_TRUE(client.messageReceived());
}
