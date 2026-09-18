#include <gtest/gtest.h>
// #include "QtRadioClientWithMocks.h"

#include <linux/test-utils/QtTransportTestRadio.h>

#include "ui/qt/QtRadioClient.h"

// using ::testing::_;

// Basic instantiation test
TEST(QtRadioClientTest, CanInstantiate) {
  QtRadioClient client(nullptr);
  EXPECT_TRUE(true); // Basic smoke test
}

// Test that we can create and destroy without crashing
TEST(QtRadioClientTest, ConstructorDestructor) {
  auto* client = new QtRadioClient(nullptr);
  EXPECT_NE(client, nullptr);
  delete client;
}

TEST(QtRadioClientTest, CanRequestAll) {
  QtRadioClient client(nullptr);

  Config::Radio::Fields clientConfig{}; // Ignored for now
  ASSERT_EQ(client.configure(clientConfig), ResultCode::OK);

  QtTransportTestRadio radio;
  ASSERT_EQ(radio.configure(), ResultCode::OK);

  ASSERT_EQ(client.start(), ResultCode::OK);
  ASSERT_EQ(radio.start(), ResultCode::OK);

  EXPECT_EQ(client.requestAll(), ResultCode::OK);

  std::this_thread::sleep_for(std::chrono::seconds(1));
  radio.stop();
  client.stop();
}

// TEST(QtRadioClientTest, CanRetrieveSettings) {
//   QtRadioClientWithMocks client(nullptr);
//
//   Config::Radio::Fields clientConfig{}; // Ignored for now
//   ASSERT_EQ(client.configure(clientConfig), ResultCode::OK);
//
//   QtTransportTestRadio radio;
//   ASSERT_EQ(radio.configure(), ResultCode::OK);
//
//   ASSERT_EQ(client.start(), ResultCode::OK);
//   ASSERT_EQ(radio.start(), ResultCode::OK);
//
//   EXPECT_CALL(client, applySettings(_)).Times(1);
//
//   ASSERT_EQ(client.testRequestCurrentSettings(), ResultCode::OK);
//
//   std::this_thread::sleep_for(std::chrono::seconds(1));
//   radio.stop();
//   client.stop();
// }
//
// TEST(QtRadioClientTest, CanRetrieveBands) {
//   QtRadioClientWithMocks client(nullptr);
//
//   Config::Radio::Fields clientConfig{}; // Ignored for now
//   ASSERT_EQ(client.configure(clientConfig), ResultCode::OK);
//
//   QtTransportTestRadio radio;
//   ASSERT_EQ(radio.configure(), ResultCode::OK);
//
//   ASSERT_EQ(client.start(), ResultCode::OK);
//   ASSERT_EQ(radio.start(), ResultCode::OK);
//
//   EXPECT_CALL(client, setBands(_)).Times(1);
//
//   ASSERT_EQ(client.testRequestBands(), ResultCode::OK);
//
//   std::this_thread::sleep_for(std::chrono::seconds(1));
//   radio.stop();
//   client.stop();
// }
//
// TEST(QtRadioClientTest, CanRetrieveModes) {
//   QtRadioClientWithMocks client(nullptr);
//
//   Config::Radio::Fields clientConfig{}; // Ignored for now
//   ASSERT_EQ(client.configure(clientConfig), ResultCode::OK);
//
//   QtTransportTestRadio radio;
//   ASSERT_EQ(radio.configure(), ResultCode::OK);
//
//   ASSERT_EQ(client.start(), ResultCode::OK);
//   ASSERT_EQ(radio.start(), ResultCode::OK);
//
//   EXPECT_CALL(client, setModes(_)).Times(1);
//
//   ASSERT_EQ(client.testRequestModes(), ResultCode::OK);
//
//   std::this_thread::sleep_for(std::chrono::seconds(1));
//   radio.stop();
//   client.stop();
// }
