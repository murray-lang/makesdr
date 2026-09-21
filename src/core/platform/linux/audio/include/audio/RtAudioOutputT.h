#pragma once

#include <CrossPlatformTypes.h>
#include "AudioOutput.h"
#include <atomic>
#include <deque>

template <typename T>
class RtAudioOutputT : public AudioOutput
{
public:

  RtAudioOutputT(RtAudio::Api api, const RtAudio::DeviceInfo& deviceInfo, const Format& format) :
    AudioOutput(api, deviceInfo, format),
    m_running(false),
    m_audioBuffer(),
    m_maxPacketFrames(0)
  {
  }
  ~RtAudioOutputT() override
  {
    RtAudioOutputT::stop();
  }

  ResultCode start(uint32_t maxPacketFrames) override
  {
    m_maxPacketFrames = maxPacketFrames;
    if (!m_running) {
      RtAudio::StreamParameters parameters;
      parameters.nChannels = 2; //std::min(m_deviceInfo.outputChannels, static_cast<unsigned int>(2));
      parameters.firstChannel = 0;

      // RtAudio::StreamOptions options{
      //   .flags = 0, //RTAUDIO_NONINTERLEAVED,
      //   .numberOfBuffers = 2
      // };
      // Set options as needed

      unsigned int sampleRate = m_format.sampleRate;

      RtAudioCallback rtCallback = [](void *outputBuffer, void *, unsigned int nFrames,
                          double, RtAudioStreamStatus, void *userData) -> int {
        auto *self = static_cast<RtAudioOutputT *>(userData);
        return self->pullSamples(outputBuffer, nFrames);
      };

      // Close any existing stream before opening a new one. Resolving the
      // device afterwards keeps our own open stream from making the device
      // look busy, and therefore unprobeable, to the re-probe.
      if (m_rtAudio->isStreamOpen()) {
        m_rtAudio->closeStream();
      }

      if (!resolveDeviceId(parameters.deviceId)) {
        return ResultCode::ERR_AUDIO_NO_MATCHING_OUTPUT_DEVICE;
      }

      RtAudioErrorType rc = m_rtAudio->openStream(
        &parameters,
        nullptr,
        static_cast<RtAudioFormat>(m_format.sampleFormat),
        sampleRate,
        &m_maxPacketFrames, rtCallback, this /*, &options*/);
      if (rc != RTAUDIO_NO_ERROR) {
        return ResultCode::ERR_AUDIO_OUTPUT_DRIVER_START_FAILED;
      }

      rc = m_rtAudio->startStream();
      if (rc != RTAUDIO_NO_ERROR) {
        m_rtAudio->closeStream();
        return ResultCode::ERR_AUDIO_OUTPUT_DRIVER_START_FAILED;
      }
      m_running = true;
      return ResultCode::OK;
    } else {
      return ResultCode::ERR_AUDIO_OUTPUT_DRIVER_ALREADY_STARTED;
    }

  }

  void stop() override
  {
    // Unconditional: a failed start() can leave m_running set with no open
    // stream, and that used to wedge the driver as permanently "started".
    m_running = false;
    if (m_rtAudio->isStreamOpen()) {
      m_rtAudio->stopStream();
      m_rtAudio->closeStream();
    }
  }

  int pullSamples(void *outputBuffer, unsigned int nFrames)
  {
    lock_guard<mutex> lock(m_mutex);

    unsigned int samplesNeeded = nFrames * m_format.channelCount;
    unsigned int samplesToCopy = std::min(static_cast<unsigned int>(m_audioBuffer.size()), samplesNeeded);

    T* out = static_cast<T*>(outputBuffer);

    for (unsigned int i = 0; i < samplesToCopy; ++i) {
      *out++ = m_audioBuffer.front();
      m_audioBuffer.pop_front();
    }

    // Fill the rest with silence if we ran out of data (underrun protection)
    if (samplesToCopy < samplesNeeded) {
      std::fill(out, out + (samplesNeeded - samplesToCopy), static_cast<T>(0));
      // Optionally log underrun here
      // qDebug() << "Audio underrun: needed" << samplesNeeded << "but got" << samplesToCopy;
    }

    return 0; // 0: continue, nonzero: stop
  }

  uint32_t addAudioData(const RealSamplesMax& data, uint32_t length, uint32_t numChannels) override
  {
    lock_guard<mutex> lock(m_mutex);
    if (!m_running) return 0;

    double scale = std::is_integral_v<T>
        ? static_cast<double>(std::numeric_limits<T>::max())
        : 1.0;
    if constexpr (std::is_same_v<T, int32_t>) {
      if (m_format.sampleFormat == AudioFormat::SINT24) {
        scale = 8388607.0; // 2^23 - 1
      }
    }

    uint32_t repeats = m_format.channelCount / numChannels;
    for (uint32_t i = 0; i < length; ++i) {
      T sample = static_cast<T>(data[i] * scale);
      for (uint32_t r = 0; r < repeats; ++r) {
        m_audioBuffer.push_back(sample);
      }
    }
    return length;
  }

private:
  atomic<bool> m_running;
  std::deque<T> m_audioBuffer;
  mutex m_mutex;
  uint32_t m_maxPacketFrames;
};