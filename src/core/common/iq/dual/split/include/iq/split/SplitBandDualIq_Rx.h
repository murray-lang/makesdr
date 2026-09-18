#pragma once

#include <iq/base/IqRxTxBaseT.h>
#include <iq/io/IqIo.h>
#include <iq/pipeline/IqRxPipeline.h>
#include <audio/mixer/AudioMixer.h>

#include "settings/model/radios/iq/RxTxDualIqBandSettings.h"


class SplitBandDualIq_Rx : public IqSink
{
public:
  SplitBandDualIq_Rx(const BandCategoryList& bands, const ModeList& modes);
  ~SplitBandDualIq_Rx() override = default;

  ResultCode configure(const Config::Sdr::Fields& sdrConfig);

  ResultCode start();
  void stop();

  ResultCode apply(IBandSettings* bandSettings);

  uint32_t sinkIq(ComplexPingPongBuffers& samples, uint32_t length) override;

  IqRxPipeline* focusPipeline(IBandSettings* bandSettings);

protected:
  IqIo m_iqIo;
  IqRxPipeline m_rxPipelineA;
  IqRxPipeline m_rxPipelineB;
  bool m_pipelineBEnabled;
  AudioMixer m_mixer;
};
