#include "audio/RtAudioFactoryBase.h"

RtAudio::Api 
RtAudioFactoryBase::apiFromConfig(const Config::Audio::Fields& config)
{
  return apiFromConfig(config.soundApi);
}

RtAudio::Api 
RtAudioFactoryBase::apiFromConfig(const Config::Audio::ShortString& configApi)
{
  if (configApi == "alsa") {
    return RtAudio::Api::LINUX_ALSA;
  }
  else if (configApi == "oss") {
    return RtAudio::Api::LINUX_OSS;
  }
  else if (configApi == "pulse") {
    return RtAudio::Api::LINUX_PULSE;
  }
  else if (configApi == "jack") {
    return RtAudio::Api::UNIX_JACK;
  }
  return RtAudio::Api::UNSPECIFIED;
}

uint32_t
RtAudioFactoryBase::getBytesPerChannel(AudioFormat rtFormat)
{
  switch (rtFormat) {
  case AudioFormat::SINT8:
    return 1;
  case AudioFormat::SINT16:
    return 2;
  case AudioFormat::SINT24:
    return 3;
  case AudioFormat::SINT32:
  case AudioFormat::FLOAT32:
    return 4;
  case AudioFormat::FLOAT64:
    return 8;
  default:
    return 0;
  }
}

bool
RtAudioFactoryBase::isRtAudioFormatSupported(RtAudioFormat format, const RtAudio::DeviceInfo& deviceInfo)
{
  return (deviceInfo.nativeFormats & format) != 0;
}

RtAudioFormat RtAudioFactoryBase::getDefaultRtAudioFormat(const RtAudio::DeviceInfo& deviceInfo)
{
  if (deviceInfo.nativeFormats & static_cast<RtAudioFormat>(AudioFormat::FLOAT32)) {
    return static_cast<RtAudioFormat>(AudioFormat::FLOAT32);
  }
  if (deviceInfo.nativeFormats & static_cast<RtAudioFormat>(AudioFormat::SINT32)) {
    return static_cast<RtAudioFormat>(AudioFormat::SINT32);
  }
  if (deviceInfo.nativeFormats & static_cast<RtAudioFormat>(AudioFormat::SINT24)) {
    return static_cast<RtAudioFormat>(AudioFormat::SINT24);
  }
  if (deviceInfo.nativeFormats & static_cast<RtAudioFormat>(AudioFormat::SINT16)) {
    return static_cast<RtAudioFormat>(AudioFormat::SINT16);
  }
  if (deviceInfo.nativeFormats & static_cast<RtAudioFormat>(AudioFormat::SINT8)) {
    return static_cast<RtAudioFormat>(AudioFormat::SINT8);
  }
  return 0;
}

RtAudioFormat
RtAudioFactoryBase::getRtAudioFormatFromConfigOrDefault(const Config::Audio::Fields& config,
                                                                      const RtAudio::DeviceInfo& deviceInfo)
{
  RtAudioFormat format = getRtAudioFormatFromConfig(config);
  if (format == 0) {
    format = getDefaultRtAudioFormat(deviceInfo);
  } //else {
  // if (!isRtAudioFormatSupported(format, deviceInfo)) {
  //   std::ostringstream stringStream;
  //   stringStream << "The format '" << pConfig->getFormat() << "' is not supported by the device '" << deviceInfo.name << "'";
  //   std::string copyOfStr = stringStream.str();
  //   throw AudioException(copyOfStr);
  // }
  // }
  return format;
}

RtAudioFormat
RtAudioFactoryBase::getRtAudioFormatFromConfig(const Config::Audio::Fields& config)
{
  return getRtAudioFormatFromConfig(config.format);
}

RtAudioFormat
RtAudioFactoryBase::getRtAudioFormatFromConfig(const Config::Audio::ShortString& configFormat)
{
  if (configFormat == "float32") {
    return static_cast<RtAudioFormat>(AudioFormat::FLOAT32);
  }
  if (configFormat == "float64") {
    return static_cast<RtAudioFormat>(AudioFormat::FLOAT64);
  }
  if (configFormat == "sint8") {
    return static_cast<RtAudioFormat>(AudioFormat::SINT8);
  }
  if (configFormat == "sint16") {
    return static_cast<RtAudioFormat>(AudioFormat::SINT16);
  }
  if (configFormat == "sint24") {
    return static_cast<RtAudioFormat>(AudioFormat::SINT24);
  }
  if (configFormat == "sint32") {
    return static_cast<RtAudioFormat>(AudioFormat::SINT32);
  }
  return 0;
}