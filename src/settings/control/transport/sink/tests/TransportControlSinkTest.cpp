#include <gtest/gtest.h>
#include <settings/control/transport/TransportControlSinkT.h>
#include <settings/model/radios/iq/SplitBandDualIqRxTxSettings.h>

// Template instantiation for IDE context and compile verification
using TestTransportControlSink = TransportControlSinkT<SplitBandDualIqRxTxSettings>;

// Basic instantiation test
TEST(QtTransportOutTest, CanInstantiate) {
  TestTransportControlSink transport;
  EXPECT_TRUE(true); // Basic smoke test
}

// Test that we can create and destroy without crashing
TEST(QtTransportOutTest, ConstructorDestructor) {
  auto* transport = new TestTransportControlSink();
  EXPECT_NE(transport, nullptr);
  delete transport;
}