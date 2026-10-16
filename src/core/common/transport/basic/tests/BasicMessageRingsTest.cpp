#include <gtest/gtest.h>
#include "RecordingNotifier.h"

TEST(BasicMessageRingsTest, NothingAttachedMessagesWaitInPipe)
{
  TestRings rings;
  ASSERT_TRUE(publish(rings.settingsToClient()));

  EXPECT_EQ(rings.settingsToClient().pending(), 1u);
}

TEST(BasicMessageRingsTest, AttachedClientIsWokenWithEachRingsPayloadType)
{
  TestRings rings;
  WakeRecorder client;
  rings.attachClient(&client);

  ASSERT_TRUE(publish(rings.settingsToClient()));
  ASSERT_TRUE(publish(rings.modesToClient()));
  ASSERT_TRUE(publish(rings.bandsToClient()));
  ASSERT_TRUE(publish(rings.rxMeteringToClient()));

  const std::vector<PayloadType> expected{
    settingsPayloadType(), PAYLOAD_TYPE_MODES, PAYLOAD_TYPE_BANDS, PAYLOAD_TYPE_RX_METERING
  };
  EXPECT_EQ(client.wakes, expected);
}

TEST(BasicMessageRingsTest, AttachedRadioIsWokenWithEachRingsPayloadType)
{
  TestRings rings;
  WakeRecorder radio;
  rings.attachRadio(&radio);

  ASSERT_TRUE(publish(rings.settingsToRadio()));
  ASSERT_TRUE(publish(rings.updateToRadio()));

  const std::vector<PayloadType> expected{settingsPayloadType(), PAYLOAD_TYPE_FIELD_UPDATE};
  EXPECT_EQ(radio.wakes, expected);
}

TEST(BasicMessageRingsTest, AttachingClientWakesForRingsFilledEarlier)
{
  TestRings rings;
  // A radio that starts first fills each pipe, then can commit nothing more,
  // so no further notification will ever come from it.
  while (publish(rings.settingsToClient())) {}
  while (publish(rings.rxMeteringToClient())) {}

  WakeRecorder client;
  rings.attachClient(&client);

  // Only the rings holding messages are woken; modes and bands are empty.
  const std::vector<PayloadType> expected{settingsPayloadType(), PAYLOAD_TYPE_RX_METERING};
  EXPECT_EQ(client.wakes, expected);
}

TEST(BasicMessageRingsTest, AttachingRadioWakesForRingsFilledEarlier)
{
  TestRings rings;
  ASSERT_TRUE(publish(rings.updateToRadio()));

  WakeRecorder radio;
  rings.attachRadio(&radio);

  const std::vector<PayloadType> expected{PAYLOAD_TYPE_FIELD_UPDATE};
  EXPECT_EQ(radio.wakes, expected);
}

TEST(BasicMessageRingsTest, DetachedClientIsNoLongerWoken)
{
  TestRings rings;
  WakeRecorder client;
  rings.attachClient(&client);
  rings.detachClient();

  ASSERT_TRUE(publish(rings.settingsToClient()));
  ASSERT_TRUE(publish(rings.rxMeteringToClient()));

  EXPECT_TRUE(client.wakes.empty());
  EXPECT_EQ(rings.settingsToClient().pending(), 1u);
}

TEST(BasicMessageRingsTest, DirectionsAreIndependent)
{
  TestRings rings;
  WakeRecorder radio;
  WakeRecorder client;
  rings.attachRadio(&radio);
  rings.attachClient(&client);

  // Settings travel both ways in separate rings, so each side is woken only
  // by its own.
  ASSERT_TRUE(publish(rings.settingsToRadio()));
  ASSERT_TRUE(publish(rings.modesToClient()));

  EXPECT_EQ(radio.wakes, (std::vector<PayloadType>{settingsPayloadType()}));
  EXPECT_EQ(client.wakes, (std::vector<PayloadType>{PAYLOAD_TYPE_MODES}));
  EXPECT_EQ(rings.settingsToClient().pending(), 0u);
}
