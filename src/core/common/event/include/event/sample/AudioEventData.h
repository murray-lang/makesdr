#pragma once

#include <CrossPlatformTypes.h>
#include <samples/SampleTypes.h>

class AudioEventData
{
public:
  AudioEventData() = default;
  AudioEventData(const RealSamplesBuffer& samples, uint32_t _length, uint32_t _sampleRate)
    : samples(samples.begin(), samples.begin() + _length)
    , length(_length)
    , sampleRate(_sampleRate)
  {}

  RealSamplesBuffer samples;
  uint32_t length;
  uint32_t sampleRate;
};