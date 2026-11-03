#ifndef LINUX_AUDIO_FACTORY_BASE_H_
#define LINUX_AUDIO_FACTORY_BASE_H_

#include <ResultCode.h>
#include <config/struct/AudioConfig.h>
#include <audio/AudioSink.h>
#include <audio/AudioBase.h>
#include <rtaudio/RtAudio.h>


#ifdef USE_ETL
#include <etl/string.h>

using InfoString = etl::string<80>;
#else
using InfoString = std::string;
#endif

class RtAudioFactoryBase
{
  public:

  static RtAudio::Api apiFromConfig(const Config::Audio::Fields& config);
  static RtAudio::Api apiFromConfig(const Config::Audio::ShortString& configApi);
  static uint32_t getBytesPerChannel(AudioFormat rtFormat);
  static bool isRtAudioFormatSupported(RtAudioFormat format, const RtAudio::DeviceInfo& deviceInfo);
  static RtAudioFormat getDefaultRtAudioFormat(const RtAudio::DeviceInfo& deviceInfo);
  static RtAudioFormat getRtAudioFormatFromConfigOrDefault(
    const Config::Audio::Fields& config,
    const RtAudio::DeviceInfo& deviceInfo
    );
  static RtAudioFormat getRtAudioFormatFromConfig(const Config::Audio::Fields& config);
  static RtAudioFormat getRtAudioFormatFromConfig(const Config::Audio::ShortString& configFormat);
};
#endif // LINUX_AUDIO_FACTORY_BASE_H_