#pragma once

#include <samples/SampleTypes.h>
#include <liquid/liquid.h>


class HilbertTransform
{
public:
  explicit HilbertTransform(int numTaps, sdrreal upperBandFraction = 0.95);
  ~HilbertTransform();

  void initialise(int numTaps, sdrreal stopBandAttenuation = 100.0);


  uint32_t transform(const RealSamplesBuffer& input, uint32_t numSamples, uint32_t numChannels, ComplexSamplesBuffer& output);

protected:
  void clearState();

protected:
  firhilbf m_hilbertState;
};

