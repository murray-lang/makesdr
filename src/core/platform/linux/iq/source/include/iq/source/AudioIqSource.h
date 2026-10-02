#ifndef LINUX_AUDIO_IQ_SOURCE_H
#define LINUX_AUDIO_IQ_SOURCE_H

#include <ResultCode.h>
#include <audio/AudioInput.h>
#include <audio/AudioSink.h>

#include <config/struct/AudioIqSourceConfig.h>
#include <samples/SampleTypes.h>
#include <iq/base/IqSource.h>
#include <dsp/transforms/hilbert/HilbertTransform.h>
#include <chrono>

class AudioIqSource : public IqSource, private AudioSink
{
public:
  AudioIqSource();

  explicit AudioIqSource(IqSink* pIqSink);
  ~AudioIqSource() override = default;

  ResultCode configure(const Config::AudioIqSource::Fields& config);

  ResultCode start(uint32_t maxPacketFrames) override;

  void stop() override;

  [[nodiscard]] uint32_t getSampleRate() const override { return m_audioInput.getSampleRate(); }

  uint32_t sinkAudio(const RealSamplesBuffer& audioSamples, uint32_t length, uint32_t numChannels) override;

protected:
  AudioInput m_audioInput;
  HilbertTransform m_hilbert;
  // ComplexPingPongBuffers m_iqOutputBuffers;
  ComplexSamplesBuffer m_outputBuffer;
  // std::chrono::steady_clock::time_point m_lastTime;
};

#endif // LINUX_AUDIO_IQ_SOURCE_H

