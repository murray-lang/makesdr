#include "audio/AudioOutputFactory.h"
#include "audio/RtAudioOutputT.h"

ResultCode
AudioOutputFactory::create(const Config::IqIo::AudioOutputConfigVariant& configVariant, AudioOutputVariant& output)
{
  if (holds_alternative<Config::Audio::Fields>(configVariant)) {
    const auto& config = get<Config::Audio::Fields>(configVariant);
    const RtAudio::Api api = apiFromConfig(config);
    RtAudio::DeviceInfo deviceInfo;
    ResultCode rc = findOutputDevice(api, config.searchExpression, &deviceInfo);
    if (rc != ResultCode::OK) return rc;
    AudioBase::Format format{};
    getOutputFormat(config, deviceInfo, format);
    if (format.sampleFormat == AudioFormat::FLOAT32) {
      output.emplace<RtAudioOutputFloat>(api, deviceInfo, format);
    }
    else if (format.sampleFormat == AudioFormat::SINT32 || format.sampleFormat == AudioFormat::SINT24) {
      output.emplace<RtAudioOutput32>(api, deviceInfo, format);
    }
    else if (format.sampleFormat == AudioFormat::SINT16) {
      output.emplace<RtAudioOutput16>(api, deviceInfo, format);
    }
    else if (format.sampleFormat == AudioFormat::SINT8) {
      output.emplace<RtAudioOutput8>(api, deviceInfo, format);
    }
    else {
      return ResultCode::ERR_AUDIO_UNKNOWN_FORMAT;
    }
  } else {
    return ResultCode::ERR_AUDIO_OUTPUT_UNKNOWN_TYPE;
  }
  return ResultCode::OK;
}

ResultCode
AudioOutputFactory::findOutputDevice(
  RtAudio::Api api,
  const Config::Audio::LongString& searchExpression,
  RtAudio::DeviceInfo* pDeviceInfo
)
{
  RtAudio audio(api);
  auto deviceIds = audio.getDeviceIds();
  if (deviceIds.empty()) {
    return ResultCode::ERR_AUDIO_NO_OUTPUT_DEVICES;
  }
  for (auto& deviceId : deviceIds) {
    RtAudio::DeviceInfo info = audio.getDeviceInfo(deviceId);
    // Only consider devices with input channels
    if (info.outputChannels > 0) {
      InfoString name(info.name.c_str());
      size_t pos = name.find(searchExpression);
      if (pos != InfoString::npos) {
        *pDeviceInfo = info;
        return ResultCode::OK;
      }
    }
  }
  return ResultCode::ERR_AUDIO_NO_MATCHING_OUTPUT_DEVICE;
}

ResultCode
AudioOutputFactory::findDefaultOutputDevice(RtAudio::Api api, RtAudio::DeviceInfo* pDeviceInfo)
{
  RtAudio audio(api);
  auto deviceIds = audio.getDeviceIds();
  if (deviceIds.empty()) {
    return ResultCode::ERR_AUDIO_NO_OUTPUT_DEVICES;
  }
  for (auto& deviceId : deviceIds) {
    RtAudio::DeviceInfo info = audio.getDeviceInfo(deviceId);
    if (info.isDefaultOutput) {
      *pDeviceInfo = info;
      return ResultCode::OK;
    }
  }
  return ResultCode::ERR_AUDIO_NO_DEFAULT_OUTPUT_DEVICE;
}

void
AudioOutputFactory::getOutputFormat(const Config::Audio::Fields& config, const RtAudio::DeviceInfo& deviceInfo,
                                         AudioBase::Format& format)
{
  format.sampleFormat = static_cast<AudioFormat>(getRtAudioFormatFromConfigOrDefault(config, deviceInfo));
  if (config.sampleRate == 0) {
    format.sampleRate = deviceInfo.preferredSampleRate;
  }
  else {
    format.sampleRate = static_cast<int>(config.sampleRate);
  }
  if (config.channelCount == 0) {
    format.channelCount = deviceInfo.outputChannels;
  }
  else {
    format.channelCount = static_cast<int>(config.channelCount);
  }
  format.bytesPerFrame = getBytesPerChannel(format.sampleFormat) * format.channelCount;
}