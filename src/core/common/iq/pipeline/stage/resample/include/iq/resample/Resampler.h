#pragma once

#include <samples/SampleTypes.h>
#include <iq/pipeline/stage/IqPipelineStage.h>
#include <liquid/liquid.h>

class Resampler: public IqPipelineStage
{
public:
  Resampler();
  ~Resampler() override;

  uint32_t configure(uint32_t inputSampleRate, uint32_t outputSampleRate);

  void initialise();

  uint32_t processSamples(
    const RealSamplesBuffer& in,
    RealSamplesBuffer& out,
    uint32_t inputLength
    ) const;

  uint32_t processSamples(
    ComplexPingPongBuffers& buffers,
    uint32_t inputLength
    ) override;

protected:
  void clearState();

protected:
  uint32_t m_inputSampleRate;
  uint32_t m_outputSampleRate;
  resamp_rrrf m_pResampleStateReal;
  resamp_crcf m_pResampleStateComplex;
  double m_ratio;
};
