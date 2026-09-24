#pragma once
#include <ResultCode.h>
#include <config/struct/IqIoConfig.h>
#include <iq/source/IqSourceVariant.h>
#include <audio/AudioOutputVariant.h>

class IqIo: public AudioSink
{
public:
  IqIo() = default;
  ~IqIo() override = default;

  ResultCode configure(const Config::IqIo::Fields& config);

  void setIqSink(IqSink* pIqSink);
  uint32_t sinkAudio(const RealSamplesBuffer& samples, uint32_t length, uint32_t numChannels) override;

  [[nodiscard]] uint32_t getInputSampleRate() const;
  [[nodiscard]] uint32_t getOutputSampleRate() const;

  [[nodiscard]] ResultCode start(uint32_t maxFramesPerInputPacket, uint32_t maxFramesPerOutputPacket);
  void stop();

protected:
  ResultCode startInput(uint32_t maxFramesPerInputPacket);
  ResultCode startOutput(uint32_t maxFramesPerOutputPacket);
  void stopInput();
  void stopOutput();

protected:
  IqSourceVariant m_iqSource;
  AudioOutputVariant m_audioOutput;
  AudioOutputBase* m_pAudioOutputAsBase = nullptr;
};
