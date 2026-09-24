#include "audio/mixer/AudioMixerInput.h"

#include "audio/mixer/AudioMixer.h"

uint32_t
AudioMixerInput::sinkAudio(const RealSamplesBuffer& samples, uint32_t length, uint32_t numChannels)
{
  return m_mixer.sinkAudioInput(m_inputId, samples, length, numChannels);
}
