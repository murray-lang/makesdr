#ifndef LINUX_AUDIO_INPUT_FACTORY_H
#define LINUX_AUDIO_INPUT_FACTORY_H

#include "AudioFactoryBase.h"
#include "AudioInput.h"

class AudioInputFactory : public AudioFactoryBase
{
public:

  static ResultCode validateConfig(const Config::Audio::Fields& config);
  static ResultCode create(const Config::Audio::Fields& config, AudioSink* pSink, AudioInput& input);

  static ResultCode findInputDevice(
    RtAudio::Api api,
    const Config::Audio::LongString& searchExpression,
    RtAudio::DeviceInfo* pDeviceInfo
  );

  static ResultCode findDefaultInputDevice(RtAudio::Api api, RtAudio::DeviceInfo* pDeviceInfo);

  static void getInputFormat(
    const Config::Audio::Fields& config,
    const RtAudio::DeviceInfo& deviceInfo,
    AudioBase::Format& format
    );
};

#endif // LINUX_AUDIO_INPUT_FACTORY_H