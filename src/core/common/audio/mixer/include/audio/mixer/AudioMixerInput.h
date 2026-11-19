#pragma once
#include <audio/AudioSink.h>

class AudioMixerBase;

class AudioMixerInput : public AudioSink
{
public:
  enum InputId
  {
    INPUT_NONE,
    INPUT_A,
    INPUT_B
  };

  AudioMixerInput(AudioMixerBase& mixer, InputId inputId)
    : m_mixer(mixer)
    , m_inputId(inputId)
  {

  }

  uint32_t sinkAudio(const RealSamplesRef& samples, uint32_t length, uint32_t numChannels) override;

protected:
  AudioMixerBase& m_mixer;
  InputId m_inputId;

};