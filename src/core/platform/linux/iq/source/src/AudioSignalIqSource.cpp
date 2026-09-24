#include "iq/source/AudioSignalIqSource.h"

AudioSignalIqSource::AudioSignalIqSource() :
    IqSource(nullptr),
    m_audioInput(static_cast<AudioSink*>(this)),
    m_reverse(false)
{}

AudioSignalIqSource::AudioSignalIqSource(IqSink* pIqSink) :
  IqSource(pIqSink),
  m_audioInput(static_cast<AudioSink*>(this)),
  m_reverse(false)
{}

// AudioSignalIqSource(const AudioSignalIqSource&&) = default;
// AudioSignalIqSource& operator=(const AudioSignalIqSource&&) = default;

ResultCode
AudioSignalIqSource::configure(const Config::AudioSignalIqSource::Fields& config)
{
  m_reverse = config.reverse;
  return AudioInputFactory::create(config.audioInput, this, m_audioInput);
}

ResultCode
AudioSignalIqSource::start(uint32_t maxPacketFrames)
{
  return m_audioInput.start(maxPacketFrames);
}
void
AudioSignalIqSource::stop()
{
  m_audioInput.stop();
}

uint32_t
AudioSignalIqSource::sinkAudio(const RealSamplesBuffer& audioSamples, uint32_t length, uint32_t numChannels)
{
  uint32_t numFrames = length/2; // Assume numChannels == 2
  if (m_pIqSink != nullptr) {
    ComplexSamplesBuffer& input = m_iqOutputBuffers.input();
    if (m_reverse) {
      for (size_t i = 0; i < numFrames; i++) {
        input.at(i) = sdrcomplex(audioSamples.at(i*2+1), audioSamples.at(i*2));
      }
    } else {
      for (size_t i = 0; i < numFrames; i++) {
        input.at(i) = sdrcomplex(audioSamples.at(i*2), audioSamples.at(i*2+1));
      }
    }
    m_pIqSink->sinkIq(m_iqOutputBuffers, numFrames);
    return length;
  }
  return 0;
}