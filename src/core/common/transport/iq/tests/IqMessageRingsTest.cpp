#include <gtest/gtest.h>
#include "RecordingNotifier.h"

TEST(IqMessageRingsTest, AttachedClientIsWokenForIqAndFftRings)
{
  TestRings rings;
  WakeRecorder client;
  rings.attachClient(&client);

  ASSERT_TRUE(publish(rings.iqToClient()));
  ASSERT_TRUE(publish(rings.spectrumToClient()));

  const std::vector<PayloadType> expected{PAYLOAD_TYPE_IQ, PAYLOAD_TYPE_SPECTRUM};
  EXPECT_EQ(client.wakes, expected);
}

TEST(IqMessageRingsTest, AttachingClientStillAttachesTheBasicRings)
{
  // attachClient() hides the base version, so must also call it.
  TestRings rings;
  WakeRecorder client;
  rings.attachClient(&client);

  ASSERT_TRUE(publish(rings.settingsToClient()));
  ASSERT_TRUE(publish(rings.rxMeteringToClient()));

  const std::vector<PayloadType> expected{settingsPayloadType(), PAYLOAD_TYPE_RX_METERING};
  EXPECT_EQ(client.wakes, expected);
}

TEST(IqMessageRingsTest, AttachingClientWakesForRingsFilledEarlier)
{
  TestRings rings;
  // A pipeline that starts before the client fills its rings, then can commit
  // nothing more, so no further notification will ever come from it.
  while (publish(rings.iqToClient())) {}
  while (publish(rings.spectrumToClient())) {}
  ASSERT_TRUE(publish(rings.modesToClient()));

  WakeRecorder client;
  rings.attachClient(&client);

  // The base rings are woken first, then the IQ ones.
  const std::vector<PayloadType> expected{PAYLOAD_TYPE_MODES, PAYLOAD_TYPE_IQ, PAYLOAD_TYPE_SPECTRUM};
  EXPECT_EQ(client.wakes, expected);
}

TEST(IqMessageRingsTest, DetachedClientIsNoLongerWokenByAnyPipe)
{
  TestRings rings;
  WakeRecorder client;
  rings.attachClient(&client);
  rings.detachClient();

  ASSERT_TRUE(publish(rings.iqToClient()));
  ASSERT_TRUE(publish(rings.spectrumToClient()));
  ASSERT_TRUE(publish(rings.bandsToClient()));

  EXPECT_TRUE(client.wakes.empty());
  EXPECT_EQ(rings.iqToClient().pending(), 1u);
}

TEST(IqMessageRingsTest, RadioIsNeverWokenByIqOrFft)
{
  TestRings rings;
  WakeRecorder radio;
  rings.attachRadio(&radio);

  ASSERT_TRUE(publish(rings.iqToClient()));
  ASSERT_TRUE(publish(rings.spectrumToClient()));
  ASSERT_TRUE(publish(rings.updateToRadio()));

  EXPECT_EQ(radio.wakes, (std::vector<PayloadType>{PAYLOAD_TYPE_FIELD_UPDATE}));
}
