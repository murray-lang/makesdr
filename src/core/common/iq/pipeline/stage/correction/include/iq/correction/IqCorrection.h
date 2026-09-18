#pragma once

#include <samples/SampleTypes.h>
#include <iq/pipeline/stage/IqPipelineStage.h>
#include <settings/model/radios/iq/IqCorrectionSettings.h>

class IqCorrection : public IqPipelineStage
{
public:
  IqCorrection();
  ~IqCorrection() override = default;

  void apply(const IqCorrectionSettings& correctionSettings);

  uint32_t processSamples(ComplexPingPongBuffers& buffers, uint32_t inputLength) override;
  uint32_t processSamples(const ComplexSamplesMax& input, ComplexSamplesMax& output, uint32_t inputLength) const;
protected:
  sdrreal m_amplitudeCorrection;
  sdrreal m_phaseCorrection;

};
