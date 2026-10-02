#pragma once

#include <CrossPlatformTypes.h>
#include <samples/SampleTypes.h>

class IqEventData
{
public:
  IqEventData() = default;
  IqEventData(const ComplexSamplesBuffer& iq, uint32_t _length, uint32_t _sampleRate)
    : samples(iq.begin(), iq.begin() + _length)
    , length(_length)
    , sampleRate(_sampleRate)
  {
  }

  ComplexSamplesBuffer samples;
  uint32_t length;
  uint32_t sampleRate;
};
