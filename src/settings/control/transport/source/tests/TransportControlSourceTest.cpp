#include <gtest/gtest.h>
#include <settings/control/transport/TransportControlSourceT.h>
#include <settings/model/radios/iq/SplitBandDualIqRxTxSettings.h>

// Template instantiation for IDE context and compile verification
using TestTransportControlSource = TransportControlSourceT<SplitBandDualIqRxTxSettings>;

// Basic instantiation test
TEST(QtTransportOutTest, CanInstantiate) {
  TestTransportControlSource transport;
  EXPECT_TRUE(true); // Basic smoke test
}

// Test that we can create and destroy without crashing
TEST(QtTransportOutTest, ConstructorDestructor) {
  auto* transport = new TestTransportControlSource();
  EXPECT_NE(transport, nullptr);
  delete transport;
}