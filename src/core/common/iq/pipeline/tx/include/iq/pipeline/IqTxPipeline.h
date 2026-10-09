#pragma once

#include <iq/pipeline/IqPipeline.h>
#include <iq/correction/IqCorrection.h>
#include <iq/resample/Resampler.h>
#include <iq/filter/FilterStage.h>


#include <iq/modulation/Modulator.h>
#include <iq/modulation/CwModulator.h>
// #include <iq/modulation/AmModulator.h>
#include <iq/modulation/FmModulator.h>
#include <iq/modulation/SsbModulator.h>
#include <settings/model/radio/iq/TxPipelineSettings.h>


class IqTxPipeline : public IqPipeline
{
public:
  IqTxPipeline(PipelineId pipelineId,const ModeList& modes);
  ~IqTxPipeline() override = default;

  void initialise(IqIo* pIo, AudioSink* pAudioSink) override;
  void setOutputSampleRate(uint32_t outputSampleRate) override;

  uint32_t processSamples(ComplexPingPongBuffers& samples, uint32_t length) override;

  ResultCode apply(const BandRfSettings* bandRfSettings, PipelineSettings* settings) override;

  [[nodiscard]] uint32_t getMaxFramesPerInputPacket() const override;
  [[nodiscard]] uint32_t getMaxFramesPerOutputPacket() const override;
  // [[nodiscard]] bool isFrequencyWithinNyquist(int64_t centreFrequency, int64_t frequency) const override;
  void calcNyquistOffsetsLimits(int32_t* maxNegative, int32_t* maxPositive) const override;

  void setMode(const Mode& mode) override;

protected:
  void setModulator(const Mode& mode);
  void setModulatorSampleRate(uint32_t sampleRate);
  static uint32_t interleaveComplexToReal(const ComplexSamplesBuffer& vcomplex, RealSamplesBuffer& vreal, uint32_t numComplexes);


private:
  IqCorrection m_iqCorrection;
  Resampler m_resampler;
  FilterStage m_ifFilter;
  // AmModulator m_amModulator;
  FmModulator m_fmnModulator;
  FmModulator m_fmwModulator;
  SsbModulator m_ssbModulator;
  CwModulator m_cwModulator;
  Modulator* m_pModulator;

  RealSamplesBuffer m_audioBuffer;
};
