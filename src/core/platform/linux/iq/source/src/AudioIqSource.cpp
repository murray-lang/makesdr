#include "iq/source/AudioIqSource.h"
#include <audio/AudioInputFactory.h>

AudioIqSource::AudioIqSource() :
  IqSource(nullptr),
  m_audioInput(dynamic_cast<AudioSink*>(this)),
  m_hilbert(63)
{
};

AudioIqSource::AudioIqSource(IqSink* pIqSink) :
  IqSource(pIqSink),
  m_audioInput(dynamic_cast<AudioSink*>(this)),
  m_hilbert(63)
{
};

ResultCode
AudioIqSource::configure(const Config::AudioIqSource::Fields& config)
{
  return AudioInputFactory::create(config.audioInput, this, m_audioInput);
}

ResultCode
AudioIqSource::start(uint32_t maxPacketFrames)
{
  // m_lastTime =  std::chrono::steady_clock::now();
  return m_audioInput.start(maxPacketFrames);
}

void
AudioIqSource::stop()
{
  m_audioInput.stop();
}

uint32_t
AudioIqSource::sinkAudio(const RealSamplesBuffer& audioSamples, uint32_t length, uint32_t numChannels) {
  // auto now = std::chrono::steady_clock::now();
  // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - m_lastTime).count();
  // m_lastTime = now;
  // qDebug() << "AudioIqSource::sinkAudio(): received" << length << "samples in" << duration << "us";

  if (m_pIqSink != nullptr) {
    uint32_t numFrames = length / numChannels;
    // m_iqOutputBuffers.resize(numFrames);
    // uint32_t outputLength = numFrames;
    uint32_t outputLength = m_hilbert.transform(audioSamples, length, numChannels, m_iqOutputBuffers.input());
    // for (int i = 0; i < numFrames; ++i) {
    //   size_t audioIndex = i * numChannels;
    //   sdrreal real = audioSamples[audioIndex] * 100; // Take first channel only for now
    //   // Hilbert transform to get imag part
    //   // m_hilbert.transform(vsdrreal(&audioSamples[audioIndex], &audioSamples[audioIndex + 1]), 1, numChannels, m_iqOutputBuffers);
    //   sdrreal imag = 0.0f;
    //   m_iqOutputBuffers[i] = sdrcomplex(real, imag);

    // }
    m_pIqSink->sinkIq(m_iqOutputBuffers, outputLength);
    return length;
  }
  return 0;
}
