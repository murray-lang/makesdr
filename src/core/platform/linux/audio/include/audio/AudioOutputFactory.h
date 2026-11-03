#ifndef LINUX_AUDIO_OUTPUT_FACTORY_H
#define LINUX_AUDIO_OUTPUT_FACTORY_H

#include "RtAudioFactoryBase.h"
#include "AudioOutput.h"
#include "AudioOutputVariant.h"
#include <config/struct/IqIoConfig.h>

class AudioOutputFactory : public RtAudioFactoryBase
{
public:
  static ResultCode create(const Config::IqIo::AudioOutputConfigVariant& config, AudioOutputVariant& output);

  static ResultCode findOutputDevice(
    RtAudio::Api api,
    const Config::Audio::LongString& searchExpression,
    RtAudio::DeviceInfo* pDeviceInfo
    );

  static ResultCode findDefaultOutputDevice(RtAudio::Api api, RtAudio::DeviceInfo* pDeviceInfo);

  static void getOutputFormat(
    const Config::Audio::Fields& config,
    const RtAudio::DeviceInfo& deviceInfo,
    AudioBase::Format& format
    );
};

#endif // LINUX_AUDIO_OUTPUT_FACTORY_H