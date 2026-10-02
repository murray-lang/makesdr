#pragma once
#include <ResultCode.h>

#include <iq/base/IqSource.h>
#include <config/struct/AudioSignalIqSourceConfig.h>
#include <audio/AudioInput.h>
#include <audio/AudioSink.h>
#include <audio/AudioInputFactory.h>
#include <samples/PingPongBuffers.h>

class AudioSignalIqSource : public IqSource, AudioSink
{
public:
  AudioSignalIqSource();
  explicit AudioSignalIqSource(IqSink* pIqSink);

  // AudioSignalIqSource(const AudioSignalIqSource&&) = default;
  // AudioSignalIqSource& operator=(const AudioSignalIqSource&&) = default;

  ~AudioSignalIqSource() override = default;

  ResultCode configure(const Config::AudioSignalIqSource::Fields& config);

  ResultCode start(uint32_t maxPacketFrames) override;
  void stop() override;

  [[nodiscard]] uint32_t getSampleRate() const override { return m_audioInput.getSampleRate(); }

  uint32_t sinkAudio(const RealSamplesBuffer& audioSamples, uint32_t length, uint32_t numChannels) override;

protected:
  AudioInput m_audioInput;
  ComplexSamplesBuffer m_outputBuffer;
  bool m_reverse;

};
