#include <gtest/gtest.h>
#include <transport/in/QtTransportInT.h>
#include <settings/model/radios/iq/SplitBandDualIqRxTxSettings.h>

// Template instantiation for IDE context and compile verification
using TestQtTransportIn = QtTransportInT<SplitBandDualIqRxTxSettings>;

// Basic instantiation test
TEST(QtTransportInTest, CanInstantiate) {
    TestQtTransportIn transport;
    EXPECT_TRUE(true); // Basic smoke test
}

// Test that we can create and destroy without crashing
TEST(QtTransportInTest, ConstructorDestructor) {
  auto* transport = new TestQtTransportIn();
  EXPECT_NE(transport, nullptr);
  delete transport;
}

// Valid Configuration test
TEST(QtTransportInTest, CanConfigure) {
  TestQtTransportIn transport;
  Config::QtTransportIn::Fields config
  {
    .message = Config::QtTransport::Settings,
    .target = Config::QtTransport::Radio,
  };

  ResultCode result = transport.configure(config);
  EXPECT_EQ(result, ResultCode::OK);
}

// Invalid Radio Configuration test
TEST(QtTransportInTest, InvalidRadioConfiguration) {
  TestQtTransportIn transport;
  Config::QtTransportIn::Fields config
  {
    .message = Config::QtTransport::Meter,
    .target = Config::QtTransport::Radio,
  };
  ResultCode result = transport.configure(config);
  EXPECT_EQ(result, ResultCode::ERR_CONFIG_INVALID_QT_TARGET_FOR_RADIO_INPUT);
}


