#ifndef LINUX_AUDIO_OUTPUT_H
#define LINUX_AUDIO_OUTPUT_H


#include <audio/AudioOutputBase.h>
#include "RtAudioDriver.h"

class AudioOutput : public AudioOutputBase, public RtAudioDriver
{
public:
  AudioOutput(RtAudio::Api api, const RtAudio::DeviceInfo& deviceInfo, const Format& format) :
    AudioOutputBase(format),
    RtAudioDriver(api, deviceInfo)
  {}

  // RtAudio owns a raw RtApi* and has no move members, so any defaulted
  // move here would silently resolve to a shallow copy and double-delete it.
  AudioOutput(const AudioOutput&)            = delete;
  AudioOutput& operator=(const AudioOutput&) = delete;
  AudioOutput(AudioOutput&&)                 = delete;
  AudioOutput& operator=(AudioOutput&&)      = delete;

  ~AudioOutput() override = default;

  uint32_t addAudioData(const RealSamplesBuffer& data, uint32_t length, uint32_t numChannels) override = 0;
};



#endif // LINUX_AUDIO_OUTPUT_H
