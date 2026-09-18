#include <gtest/gtest.h>
#include <transport/out/QtTransportOutT.h>
#include <settings/model/radios/iq/SplitBandDualIqRxTxSettings.h>

// Template instantiation for IDE context and compile verification
using TestQtTransportOut = QtTransportOutT<SplitBandDualIqRxTxSettings>;

// Basic instantiation test
TEST(QtTransportOutTest, CanInstantiate) {
    TestQtTransportOut transport;
    EXPECT_TRUE(true); // Basic smoke test
}

// Test that we can create and destroy without crashing
TEST(QtTransportOutTest, ConstructorDestructor) {
  auto* transport = new TestQtTransportOut();
  EXPECT_NE(transport, nullptr);
  delete transport;
}

// Configuration test
TEST(QtTransportOutTest, CanConfigure) {
    TestQtTransportOut transport;
    Config::QtTransportOut::Fields config;

    ResultCode result = transport.configure(config);
    EXPECT_EQ(result, ResultCode::OK);
}

// Invalid Radio Configuration test
TEST(QtTransportOutTest, InvalidClientConfiguration) {
  TestQtTransportOut transport;
  Config::QtTransportOut::Fields config
  {
    .message = Config::QtTransport::Meter,
    .target = Config::QtTransport::Client,
  };
  ResultCode result = transport.configure(config);
  EXPECT_EQ(result, ResultCode::ERR_CONFIG_INVALID_QT_TARGET_FOR_CLIENT_OUTPUT);
}


