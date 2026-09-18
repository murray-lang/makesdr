#pragma once

#include <iq/pipeline/IqPipeline.h>
#include <iq/correction/DcShift.h>
#include <iq/correction/IqCorrection.h>
#include <iq/resample/Resampler.h>
#include <iq/filter/FilterStage.h>
#include <iq/monitor/MonitorStageT.h>


#include <iq/modulation/Demodulator.h>
#include <iq/modulation/CwDemodulator.h>
#include <iq/modulation/AmDemodulator.h>
#include <iq/modulation/FmDemodulator.h>
#include <iq/modulation/SsbDemodulator.h>

#include <event/EventDispatcher.h>
#include <event/sample/RxIqEvent.h>
#include <settings/model/radios/iq/RxPipelineSettings.h>


class IqRxPipeline : public IqPipeline
{
public:
  IqRxPipeline(const ModeList& modes);
  ~IqRxPipeline() override = default;

  void initialise(IqIo* pIo, AudioSink* pAudioSink) override;
  void setOutputSampleRate(uint32_t outputSampleRate) override;

  uint32_t sinkIq(ComplexPingPongBuffers& samples, uint32_t length) override;

  ResultCode apply(const BandRfSettings* bandRfSettings, RxPipelineSettings* settings);

  [[nodiscard]] uint32_t getMaxFramesPerInputPacket() const override;
  [[nodiscard]] uint32_t getMaxFramesPerOutputPacket() const override;
  // [[nodiscard]] bool isFrequencyWithinNyquist(int64_t centreFrequency, int64_t frequency) const override;
  void calcNyquistOffsetsLimits(int32_t* maxNegative, int32_t* maxPositive) const override;

  void setMode(const Mode& mode) override;

protected:
  void setDemodulator(const Mode& mode);

private:
  DcShift m_dcShift;
  IqCorrection m_iqCorrection;
  Resampler m_resampler;
  FilterStage m_ifFilter;
  AmDemodulator m_amDemodulator;
  FmDemodulator m_fmnDemodulator;
  FmDemodulator m_fmwDemodulator;
  SsbDemodulator m_ssbDemodulator;
  CwDemodulator m_cwDemodulator;
  Demodulator* m_pDemodulator;
  // MonitorStageT<EventDispatcher, RxIqEvent> m_monitorStage;

  RealSamplesMax m_audioBuffer;
};
