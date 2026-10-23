#include <gtest/gtest.h>
#include "RecordingNotifier.h"

#include <transport/MetaClientSideRingsT.h>
#include <transport/MetaRadioSideRingsT.h>
#include <../../metering/include/transport/RxMeteringPublisher.h>

namespace
{
  // Stands in for whatever an interface delivers to: records the source of
  // each message.
  template<typename MessageT>
  struct RecordingSink : public MessageSinkT<MessageT>
  {
    std::vector<PayloadSource> sources;

    ResultCode applyMessage(MessageT* message) override
    {
      sources.push_back(message->source());
      return ResultCode::OK;
    }
  };

  // Also records each message's purpose, which travels in its header.
  struct SettingsSink : public RecordingSink<BasicRxSettings>
  {
    std::vector<PayloadPurpose> purposes;

    ResultCode applyMessage(BasicRxSettings* message) override
    {
      purposes.push_back(message->purpose());
      return RecordingSink::applyMessage(message);
    }
  };

  // Also keeps a copy of the last message, for checking its contents.
  struct MeteringSink : public RecordingSink<RxMeteringMessage>
  {
    RxMeteringMessage last;

    ResultCode applyMessage(RxMeteringMessage* message) override
    {
      last = *message;
      return RecordingSink::applyMessage(message);
    }
  };

  using RadioSide = MetaRadioSideRingsT<TestRings>;
  using ClientSide = MetaClientSideRingsT<TestRings>;
}

// Both ends over one set of rings, each attached to its own recorder. The
// tests play the platform glue: they read each recorder's wakes and pass them
// to the matching dispatch().
class MetaSideRingsTest : public ::testing::Test
{
protected:
  TestRings    rings;
  RadioSide    radio{rings};
  ClientSide   client{rings};
  WakeRecorder radioWakes;
  WakeRecorder clientWakes;

  void SetUp() override
  {
    radio.attach(&radioWakes);
    client.attach(&clientWakes);
  }

  void TearDown() override
  {
    radio.detach();
    client.detach();
  }

  static void deliver(WakeRecorder& wakes, auto& side)
  {
    for (PayloadType payloadType : wakes.wakes) {
      side.dispatch(payloadType);
    }
    wakes.wakes.clear();
  }

  void deliverToRadio()  { deliver(radioWakes, radio); }
  void deliverToClient() { deliver(clientWakes, client); }
};

//--- radio -> client ---------------------------------------------------------

TEST_F(MetaSideRingsTest, SettingsSentByRadioReachClientStampedBackEnd)
{
  SettingsSink sink;
  client.connectRadioSettingsSink(&sink);

  BasicRxSettings settings;
  settings.setPurpose(PayloadPurpose::PURPOSE_MERGE);
  ASSERT_EQ(radio.send(&settings), ResultCode::OK);
  deliverToClient();

  EXPECT_EQ(sink.sources, (std::vector<PayloadSource>{PayloadSource::SOURCE_BACK_END}));
  // Settings copy only their body, so the header is copied separately.
  EXPECT_EQ(sink.purposes, (std::vector<PayloadPurpose>{PayloadPurpose::PURPOSE_MERGE}));
}

TEST_F(MetaSideRingsTest, ModesAndBandsSentByRadioReachTheirOwnSinks)
{
  RecordingSink<ModeList> modesSink;
  RecordingSink<BandCategoryList> bandsSink;
  client.connectModesSink(&modesSink);
  client.connectBandsSink(&bandsSink);

  ModeList modes;
  BandCategoryList bands;
  ASSERT_EQ(radio.send(&modes), ResultCode::OK);
  ASSERT_EQ(radio.send(&bands), ResultCode::OK);
  deliverToClient();

  EXPECT_EQ(modesSink.sources, (std::vector<PayloadSource>{PayloadSource::SOURCE_BACK_END}));
  EXPECT_EQ(bandsSink.sources, (std::vector<PayloadSource>{PayloadSource::SOURCE_BACK_END}));
}

TEST_F(MetaSideRingsTest, RxMeteringSentByRadioArrivesIntact)
{
  MeteringSink sink;
  client.connectRxMeteringSink(&sink);

  RxMeteringMessage metering;
  metering.initialise(PipelineId::B, -73.5f, 12.25f);
  ASSERT_EQ(radio.send(&metering), ResultCode::OK);
  deliverToClient();

  ASSERT_EQ(sink.sources.size(), 1u);
  EXPECT_EQ(sink.last.pipelineId(), PipelineId::B);
  EXPECT_FLOAT_EQ(sink.last.rssiDbFs(), -73.5f);
  EXPECT_FLOAT_EQ(sink.last.agcGainDb(), 12.25f);
}

TEST_F(MetaSideRingsTest, RxMeteringPublishedInPlaceReachesClient)
{
  // How a pipeline stage publishes: straight into a slot, with no copy.
  MessageRingSinkT<TestRings::RxMeteringRing> pipeSink(rings.rxMeteringToClient());
  RxMeteringPublisher& publisher = pipeSink;
  MeteringSink sink;
  client.connectRxMeteringSink(&sink);

  RxMeteringMessage* pmsg = publisher.reserve();
  ASSERT_NE(pmsg, nullptr);
  pmsg->initialise(PipelineId::A, -40.0f, 3.0f);
  publisher.commit(pmsg);
  deliverToClient();

  ASSERT_EQ(sink.sources.size(), 1u);
  EXPECT_EQ(sink.last.pipelineId(), PipelineId::A);
  EXPECT_FLOAT_EQ(sink.last.rssiDbFs(), -40.0f);
}

TEST_F(MetaSideRingsTest, RadioSendToFullPipeIsRefused)
{
  MeteringSink sink;
  client.connectRxMeteringSink(&sink);

  RxMeteringMessage metering;
  ResultCode rc = ResultCode::OK;
  int sent = 0;
  while ((rc = radio.send(&metering)) == ResultCode::OK) {
    ++sent;
  }
  EXPECT_EQ(rc, ResultCode::ERR_CHANNEL_FULL);
  ASSERT_GT(sent, 0);

  // Draining makes room again.
  deliverToClient();
  EXPECT_EQ(static_cast<int>(sink.sources.size()), sent);
  EXPECT_EQ(radio.send(&metering), ResultCode::OK);
}

//--- client -> radio ---------------------------------------------------------

TEST_F(MetaSideRingsTest, SettingsSentByClientReachRadioStampedFrontEnd)
{
  SettingsSink sink;
  radio.connectRadioSettingsSink(&sink);

  BasicRxSettings settings;
  settings.setPurpose(PayloadPurpose::PURPOSE_REPLACE);
  ASSERT_EQ(client.send(&settings), ResultCode::OK);
  deliverToRadio();

  EXPECT_EQ(sink.sources, (std::vector<PayloadSource>{PayloadSource::SOURCE_FRONT_END}));
  EXPECT_EQ(sink.purposes, (std::vector<PayloadPurpose>{PayloadPurpose::PURPOSE_REPLACE}));
}

TEST_F(MetaSideRingsTest, SettingsSentByClientNeverReachTheClientsOwnSink)
{
  // Settings travel both ways, so this is the one payload type both sides
  // dispatch: each must read only the pipe addressed to it.
  SettingsSink radioSink;
  SettingsSink clientSink;
  radio.connectRadioSettingsSink(&radioSink);
  client.connectRadioSettingsSink(&clientSink);

  BasicRxSettings settings;
  ASSERT_EQ(client.send(&settings), ResultCode::OK);
  client.dispatch(settingsPayloadType());
  deliverToRadio();

  EXPECT_TRUE(clientSink.sources.empty());
  EXPECT_EQ(radioSink.sources.size(), 1u);
}

TEST_F(MetaSideRingsTest, FieldUpdatesSentByClientReachRadioStampedFrontEnd)
{
  RecordingSink<FieldUpdateMessage> sink;
  radio.connectFieldUpdateSink(&sink);

  FieldUpdateMessage message;
  ASSERT_EQ(client.send(&message), ResultCode::OK);
  ASSERT_EQ(client.applyFieldUpdate(FieldUpdate(FieldUpdateMessage{})), ResultCode::OK);
  deliverToRadio();

  EXPECT_EQ(sink.sources, (std::vector<PayloadSource>{
    PayloadSource::SOURCE_FRONT_END, PayloadSource::SOURCE_FRONT_END
  }));
}

//--- dispatch ----------------------------------------------------------------

TEST_F(MetaSideRingsTest, DispatchIgnoresPayloadTypesItDoesNotOwn)
{
  RecordingSink<ModeList> modesSink;
  RecordingSink<FieldUpdateMessage> updateSink;
  client.connectModesSink(&modesSink);
  radio.connectFieldUpdateSink(&updateSink);

  ModeList modes;
  FieldUpdateMessage update;
  ASSERT_EQ(radio.send(&modes), ResultCode::OK);
  ASSERT_EQ(client.send(&update), ResultCode::OK);
  clientWakes.wakes.clear();
  radioWakes.wakes.clear();

  // Each side reads only the rings addressed to it, and IQ is not carried by
  // basic rings at all.
  client.dispatch(PAYLOAD_TYPE_FIELD_UPDATE);
  client.dispatch(PAYLOAD_TYPE_IQ);
  radio.dispatch(PAYLOAD_TYPE_MODES);

  EXPECT_TRUE(modesSink.sources.empty());
  EXPECT_TRUE(updateSink.sources.empty());
  EXPECT_EQ(rings.modesToClient().pending(), 1u);
  EXPECT_EQ(rings.updateToRadio().pending(), 1u);
}

TEST_F(MetaSideRingsTest, PendingMessagesAreDiscardedWithNoSinkConnected)
{
  ModeList modes;
  ASSERT_EQ(radio.send(&modes), ResultCode::OK);
  deliverToClient();

  // The slot went back into circulation, so the single-slot pipe accepts more.
  EXPECT_EQ(rings.modesToClient().pending(), 0u);
  EXPECT_EQ(radio.send(&modes), ResultCode::OK);
}
