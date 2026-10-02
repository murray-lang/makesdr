
#include "iq/io/IqIo.h"
#include <iq/source/IqSourceFactory.h>
#include <audio/AudioOutputFactory.h>

ResultCode IqIo::configure(const Config::IqIo::Fields& config)
{
  ResultCode rc = IqSourceFactory::create(config.iqSource, m_iqSource);
  if (rc != ResultCode::OK) return rc;

  rc = AudioOutputFactory::create(config.audioOutput, m_audioOutput);
  if (rc != ResultCode::OK) return rc;

  // Cache the AudioOutputBase pointer for fast access in sinkAudio()
  m_pAudioOutputAsBase = visit([]<typename T0>(T0& arg) -> AudioOutputBase* {
    using T = decay_t<T0>;
    if constexpr (is_same_v<T, monostate>) {
      return nullptr;
    } else {
      return &arg;
    }
  }, m_audioOutput);

  return ResultCode::OK;
}

void
IqIo::setIqSink(IqSink* pIqSink)
{
  visit([pIqSink]<typename T0>(T0&& arg)
  {
    using T = decay_t<T0>;
    if constexpr (!is_same_v<T, monostate>) {
      arg.setIqSink(pIqSink);
    }
  }, m_iqSource);
}

uint32_t
IqIo::sinkAudio(const RealSamplesBuffer& samples, uint32_t length, uint32_t numChannels)
{
  if (m_pAudioOutputAsBase) {
    return m_pAudioOutputAsBase->addAudioData(samples, length, numChannels);
  }
  return 0;
}

uint32_t
IqIo::getInputSampleRate() const
{
  return visit([]<typename T0>(const T0& arg) -> uint32_t {
    using T = decay_t<T0>;

    if constexpr (is_same_v<T, monostate>) {
      return 0;
    } else {
      return arg.getSampleRate();
    }
  }, m_iqSource);
}

uint32_t
IqIo::getOutputSampleRate() const
{
  return visit([]<typename T0>(const T0& arg) -> uint32_t
  {
    using T = decay_t<T0>;

    if constexpr (is_same_v<T, monostate>) {
      return 0;
    } else {
      return arg.getSampleRate();
    }
  }, m_audioOutput);
}

ResultCode
IqIo::start(uint32_t maxFramesPerInputPacket, uint32_t maxFramesPerOutputPacket)
{
  ResultCode rc = startOutput(maxFramesPerOutputPacket);
  if (rc != ResultCode::OK) return rc;
  return startInput(maxFramesPerInputPacket);
}

void
IqIo::stop()
{
  stopInput();
  stopOutput();
}

ResultCode
IqIo::startInput(uint32_t maxFramesPerInputPacket)
{
  return visit([maxFramesPerInputPacket]<typename T0>(T0& arg) -> ResultCode {
    using T = decay_t<T0>;

    if constexpr (is_same_v<T, monostate>) {
      return ResultCode::ERR_IQ_SOURCE_MISSING;
    } else {
      return arg.start(maxFramesPerInputPacket);
    }
  }, m_iqSource);
}

ResultCode
IqIo::startOutput(uint32_t maxFramesPerOutputPacket)
{
  return visit([maxFramesPerOutputPacket]<typename T0>(T0& arg) -> ResultCode {
    using T = decay_t<T0>;

    if constexpr (is_same_v<T, monostate>) {
      return ResultCode::ERR_AUDIO_OUTPUT_MISSING;
    } else {
      return arg.start(maxFramesPerOutputPacket);
    }
  }, m_audioOutput);
}

void
IqIo::stopInput()
{
  visit([]<typename T0>(T0& arg) -> void {
    using T = decay_t<T0>;
    if constexpr (!is_same_v<T, monostate>) {
      arg.stop();
    }
  }, m_iqSource);
}

void
IqIo::stopOutput()
{
  visit([]<typename T0>(T0& arg) -> void {
    using T = decay_t<T0>;
    if constexpr (!is_same_v<T, monostate>) {
      arg.stop();
    }
  }, m_audioOutput);
}
