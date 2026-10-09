#pragma once

#include <iq/base/IqRxTxBaseT.h>
#include <iq/io/IqIo.h>
#include <iq/pipeline/IqRxPipeline.h>
#include <audio/mixer/AudioMixer.h>

#include <settings/model/radio/iq/RxTxDualIqBandSettings.h>
#include <transport/radio/IRadioPublishers.h>


class SplitBandDualIq_Rx : public IqSink
{
public:
  SplitBandDualIq_Rx(
    ComplexPingPongBuffers& pingPongBuffers,
    const BandCategoryList& bands,
    const ModeList& modes,
    IRadioPublishers* radioPublishers
    );
  ~SplitBandDualIq_Rx() override;

  ResultCode configure(const Config::Sdr::Fields& sdrConfig);

  ResultCode start();
  void stop();

  ResultCode apply(IBandSettings* bandSettings);

  uint32_t sinkIq(ComplexSamplesBuffer& samples, uint32_t length) override;

  IqRxPipeline* focusPipeline(IBandSettings* bandSettings);

protected:
  IqIo m_iqIo;
  ComplexPingPongBuffers& m_pingPongBuffers;
  IqRxPipeline m_rxPipelineA;
  IqRxPipeline m_rxPipelineB;
  bool m_pipelineBEnabled;
  AudioMixer m_mixer;
};
