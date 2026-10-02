#pragma once
#include <samples/SampleTypes.h>

class AudioSink
{
public:
  virtual ~AudioSink() = default;
  virtual uint32_t sinkAudio(const RealSamplesBuffer& samples, uint32_t length, uint32_t numChannels) = 0;
};
