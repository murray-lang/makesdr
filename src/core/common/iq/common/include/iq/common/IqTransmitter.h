#pragma once

#include <iq/io/IqIo.h>
#include <iq/pipeline/IqTxPipeline.h>
#include <config/struct/SdrConfig.h>


class IqTransmitter :  public IqSink, public AudioSink
{
public:
  IqTransmitter(ComplexPingPongBuffers& pingPongBuffers, const ModeList& modes);
  ~IqTransmitter() override = default;

  ResultCode configure(const Config::Sdr::Fields& sdrConfig);

  ResultCode start();
  void stop();

  ResultCode apply(const BandRfSettings* bandRfSettings, PipelineSettings* settings);

  uint32_t sinkIq(ComplexSamplesBuffer& samples, uint32_t length) override;
  uint32_t sinkAudio(const RealSamplesBuffer& samples, uint32_t length, uint32_t numChannels) override;

protected:
  ComplexPingPongBuffers& m_pingPongBuffers;
  IqIo m_iqIo;
  IqTxPipeline m_txPipeline;
};
