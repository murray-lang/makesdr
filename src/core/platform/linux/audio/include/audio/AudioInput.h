#ifndef LINUX_AUDIO_INPUT_H
#define LINUX_AUDIO_INPUT_H

#include <ResultCode.h>
#include <CrossPlatformTypes.h>
#include <thread/Runnable.h>
#include <thread/Thread.h>
#include <thread/Mutex.h>
#include <thread/WaitCondition.h>
#include <audio/AudioInputBase.h>
#include <audio/AudioSink.h>
#include <etl/deque.h>
#include "RtAudioDriver.h"

class AudioInput : public AudioInputBase, public RtAudioDriver, public Runnable
{

public:
  AudioInput();
  AudioInput(AudioSink* pSink);
  AudioInput(RtAudio::Api api, const RtAudio::DeviceInfo& deviceInfo, const Format& format, AudioSink* pSink);
  // AudioInput(AudioInput&& other) noexcept;

  ~AudioInput() override;

  // AudioInput& operator=(AudioInput&& other) noexcept;

  void configure(RtAudio::Api api, const RtAudio::DeviceInfo& deviceInfo, const Format& format, AudioSink* pSink);

  [[nodiscard]] uint32_t getMaxChannels() const override {
    return m_deviceInfo.inputChannels;
  }

  [[nodiscard]] uint32_t getNumChannels() const override {
    return m_format.channelCount;
  }

  ResultCode start(uint32_t maxPacketFrames) override;
  void stop() override;

  static int rtCallback(void *, void *inputBuffer, unsigned int nframes, double,
                                   RtAudioStreamStatus, void *userData);


  int handleCallback(void *inputBuffer, unsigned int nframes);

  void run() override;

  void getSamplesFromBuffer(size_t numFrames, uint32_t channelCount, RealSamplesBuffer& input);

private:
  Thread m_thread;
  std::atomic<bool> m_running;
  // mutex m_mutex;
  etl::deque<float, PIPELINE_BUFFER_LENGTH*2> m_queue;
  RtAudio::StreamParameters m_params;

  AudioSink* m_pSink;
  RealSamplesBuffer m_outputBuffer;
  uint32_t m_maxPacketFrames;
  uint32_t m_numCurrentFrames;
  Mutex m_mutex;
  WaitCondition m_dataAvailable;
};

#endif // LINUX_AUDIO_INPUT_H
