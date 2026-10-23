#include <gtest/gtest.h>
#include "RecordingNotifier.h"

#include <transport/IqClientSideRingsT.h>
#include <transport/IqRadioSideRingsT.h>

namespace
{
  // Stands in for whatever an interface delivers to: records the source of
  // each message, and keeps a copy of the last one.
  template<typename MessageT>
  struct RecordingSink : public MessageSinkT<MessageT>
  {
    std::vector<PayloadSource> sources;
    MessageT last;

    ResultCode applyMessage(MessageT* message) override
    {
      sources.push_back(message->source());
      last = *message;
      return ResultCode::OK;
    }
  };

  using RadioSide = IqRadioSideRingsT<TestRings>;
  using ClientSide = IqClientSideRingsT<TestRings>;

  ComplexSamplesBuffer threeSamples()
  {
    ComplexSamplesBuffer samples;
    samples.push_back({1.0f, 2.0f});
    samples.push_back({3.0f, 4.0f});
    samples.push_back({5.0f, 6.0f});
    return samples;
  }
}

// Both ends over one set of rings, each attached to its own recorder. The
// tests play the platform glue: they read each recorder's wakes and pass them
// to the matching dispatch().
class IqSideRingsTest : public ::testing::Test
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

//--- IQ and FFT --------------------------------------------------------------

TEST_F(IqSideRingsTest, IqSentByRadioArrivesIntactStampedBackEnd)
{
  RecordingSink<IqMessage> sink;
  client.connectIqSink(&sink);

  IqMessage iq(PipelineId::B, threeSamples(), 3, 48000);
  ASSERT_EQ(radio.send(&iq), ResultCode::OK);
  deliverToClient();

  ASSERT_EQ(sink.sources, (std::vector<PayloadSource>{PayloadSource::SOURCE_BACK_END}));
  EXPECT_EQ(sink.last.pipelineId(), PipelineId::B);
  EXPECT_EQ(sink.last.sampleRate(), 48000u);
  // Interleaved I and Q, so two floats per sample.
  ASSERT_EQ(sink.last.length(), 6u);
  const std::vector<float> interleaved(sink.last.data(), sink.last.data() + 6);
  EXPECT_EQ(interleaved, (std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f}));
}

TEST_F(IqSideRingsTest, FftSentByRadioArrivesIntactStampedBackEnd)
{
  RecordingSink<SpectrumMessage> sink;
  client.connectSpectrumSink(&sink);

  SpectrumMessage fft;
  fft.initialise(PipelineId::A, 96000, -120.0f, -20.0f);
  fft.bins().size = 2;
  fft.bins().bytes[0] = 7;
  fft.bins().bytes[1] = 200;
  ASSERT_EQ(radio.send(&fft), ResultCode::OK);
  deliverToClient();

  ASSERT_EQ(sink.sources, (std::vector<PayloadSource>{PayloadSource::SOURCE_BACK_END}));
  EXPECT_EQ(sink.last.pipelineId(), PipelineId::A);
  EXPECT_EQ(sink.last.sampleRate(), 96000u);
  EXPECT_FLOAT_EQ(sink.last.floor(), -120.0f);
  EXPECT_FLOAT_EQ(sink.last.ceiling(), -20.0f);
  ASSERT_EQ(sink.last.bins().size, 2u);
  EXPECT_EQ(sink.last.bins().bytes[0], 7);
  EXPECT_EQ(sink.last.bins().bytes[1], 200);
}

TEST_F(IqSideRingsTest, PublishersWriteInPlaceForPipelineStages)
{
  // How pipeline stages publish: through IRadioPublishers, straight into a
  // slot, with no copy.
  IRadioPublishers& publishers = radio;
  RecordingSink<IqMessage> iqSink;
  RecordingSink<SpectrumMessage> fftSink;
  RecordingSink<RxMeteringMessage> meteringSink;
  client.connectIqSink(&iqSink);
  client.connectSpectrumSink(&fftSink);
  client.connectRxMeteringSink(&meteringSink);

  IqMessage* iq = publishers.getIqPublisher()->reserve();
  ASSERT_NE(iq, nullptr);
  iq->initialise(PipelineId::A, threeSamples(), 2, 24000);
  publishers.getIqPublisher()->commit(iq);

  SpectrumMessage* fft = publishers.getSpectrumPublisher()->reserve();
  ASSERT_NE(fft, nullptr);
  fft->initialise(PipelineId::B, 24000, -100.0f, 0.0f);
  publishers.getSpectrumPublisher()->commit(fft);

  RxMeteringMessage* metering = publishers.getRxMeteringPublisher()->reserve();
  ASSERT_NE(metering, nullptr);
  metering->initialise(PipelineId::A, -55.0f, 6.0f);
  publishers.getRxMeteringPublisher()->commit(metering);

  deliverToClient();

  ASSERT_EQ(iqSink.sources.size(), 1u);
  EXPECT_EQ(iqSink.last.length(), 4u);
  ASSERT_EQ(fftSink.sources.size(), 1u);
  EXPECT_EQ(fftSink.last.pipelineId(), PipelineId::B);
  ASSERT_EQ(meteringSink.sources.size(), 1u);
  EXPECT_FLOAT_EQ(meteringSink.last.rssiDbFs(), -55.0f);
}

TEST_F(IqSideRingsTest, FullIqPipeRefusesUntilDrained)
{
  RecordingSink<IqMessage> sink;
  client.connectIqSink(&sink);
  IqPublisher* publisher = radio.getIqPublisher();

  IqMessage* first = publisher->reserve();
  ASSERT_NE(first, nullptr);
  publisher->commit(first);

  // A stage that finds the pipe full skips that block rather than waiting.
  EXPECT_EQ(publisher->reserve(), nullptr);

  deliverToClient();
  EXPECT_EQ(sink.sources.size(), 1u);
  EXPECT_NE(publisher->reserve(), nullptr);
}

//--- inherited behaviour -----------------------------------------------------

TEST_F(IqSideRingsTest, ClientStillDispatchesBasicRings)
{
  // dispatch() hides the base version, so must hand it everything else.
  RecordingSink<SingleIqRxSettings> settingsSink;
  RecordingSink<ModeList> modesSink;
  RecordingSink<BandCategoryList> bandsSink;
  RecordingSink<RxMeteringMessage> meteringSink;
  client.connectRadioSettingsSink(&settingsSink);
  client.connectModesSink(&modesSink);
  client.connectBandsSink(&bandsSink);
  client.connectRxMeteringSink(&meteringSink);

  // The radio's IQ send() overloads must not hide the base's.
  SingleIqRxSettings settings;
  ModeList modes;
  BandCategoryList bands;
  RxMeteringMessage metering;
  ASSERT_EQ(radio.send(&settings), ResultCode::OK);
  ASSERT_EQ(radio.send(&modes), ResultCode::OK);
  ASSERT_EQ(radio.send(&bands), ResultCode::OK);
  ASSERT_EQ(radio.send(&metering), ResultCode::OK);
  deliverToClient();

  const std::vector<PayloadSource> once{PayloadSource::SOURCE_BACK_END};
  EXPECT_EQ(settingsSink.sources, once);
  EXPECT_EQ(modesSink.sources, once);
  EXPECT_EQ(bandsSink.sources, once);
  EXPECT_EQ(meteringSink.sources, once);
}

TEST_F(IqSideRingsTest, ClientToRadioIsUnchanged)
{
  RecordingSink<SingleIqRxSettings> settingsSink;
  RecordingSink<FieldUpdateMessage> updateSink;
  radio.connectRadioSettingsSink(&settingsSink);
  radio.connectFieldUpdateSink(&updateSink);

  SingleIqRxSettings settings;
  FieldUpdateMessage update;
  ASSERT_EQ(client.send(&settings), ResultCode::OK);
  ASSERT_EQ(client.send(&update), ResultCode::OK);
  deliverToRadio();

  const std::vector<PayloadSource> once{PayloadSource::SOURCE_FRONT_END};
  EXPECT_EQ(settingsSink.sources, once);
  EXPECT_EQ(updateSink.sources, once);
}

TEST_F(IqSideRingsTest, RadioIgnoresIqAndFftPayloadTypes)
{
  RecordingSink<IqMessage> sink;
  client.connectIqSink(&sink);

  IqMessage iq;
  ASSERT_EQ(radio.send(&iq), ResultCode::OK);
  clientWakes.wakes.clear();

  // The IQ pipe is client-bound; the radio's dispatch must leave it alone.
  radio.dispatch(PAYLOAD_TYPE_IQ);
  radio.dispatch(PAYLOAD_TYPE_SPECTRUM);

  EXPECT_TRUE(sink.sources.empty());
  EXPECT_EQ(rings.iqToClient().pending(), 1u);
}
