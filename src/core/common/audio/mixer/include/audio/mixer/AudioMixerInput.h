#pragma once
#include <audio/AudioSink.h>

class AudioMixer;

class AudioMixerInput : public AudioSink
{
public:
  enum InputId
  {
    INPUT_NONE,
    INPUT_A,
    INPUT_B
  };

  AudioMixerInput(AudioMixer& mixer, InputId inputId)
    : m_mixer(mixer)
    , m_inputId(inputId)
  {

  }

  uint32_t sinkAudio(const RealSamplesBuffer& samples, uint32_t length, uint32_t numChannels) override;

protected:
  AudioMixer& m_mixer;
  InputId m_inputId;

};