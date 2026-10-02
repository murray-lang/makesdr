#pragma once
#include "AudioBase.h"
#include <samples/SampleTypes.h>

class AudioOutputBase : public AudioBase
{
public:
  AudioOutputBase(const Format& format) : AudioBase(format) {}
  virtual uint32_t addAudioData(const RealSamplesBuffer& data, uint32_t length, uint32_t numChannels) = 0;
};