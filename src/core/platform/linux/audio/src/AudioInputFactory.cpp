#include "audio/AudioInputFactory.h"

ResultCode
AudioInputFactory::create(const Config::Audio::Fields& config, AudioSink* pSink, AudioInput& input)
{
  const RtAudio::Api api = apiFromConfig(config);
  if (api == RtAudio::UNSPECIFIED) {
    return ResultCode::ERR_AUDIO_UNKNOWN_API;
  }
  RtAudio::DeviceInfo deviceInfo;
  ResultCode rc = findInputDevice(api, config.searchExpression, &deviceInfo);
  if (rc != ResultCode::OK) return rc;

  AudioBase::Format format{};
  getInputFormat(config, deviceInfo, format);
  // format.channelCount = pConfig->getChannelCount();
  format.sampleFormat = AudioFormat::FLOAT32;
  format.bytesPerFrame = sizeof(float) * format.channelCount;
  input.configure(api, deviceInfo, format, pSink); // = AudioInput(deviceInfo, format, pSink);
  return ResultCode::OK;
}

ResultCode
AudioInputFactory::findInputDevice(
  RtAudio::Api api,
  const Config::Audio::LongString& searchExpression,
  RtAudio::DeviceInfo* pDeviceInfo
)
{
  RtAudio audio(api);
  auto deviceIds = audio.getDeviceIds();
  if (deviceIds.empty()) {
    return ResultCode::ERR_AUDIO_NO_INPUT_DEVICES;
  }
  for (auto& deviceId : deviceIds) {
    RtAudio::DeviceInfo info = audio.getDeviceInfo(deviceId);
    // Only consider devices with input channels
    if (info.inputChannels > 0) {
      InfoString name(info.name.c_str());
      // qDebug() << "Found input device: " << QString::fromStdString(name);
      size_t pos = name.find(searchExpression);
      if (pos != InfoString::npos) {
        *pDeviceInfo = info;
        return ResultCode::OK;
      }
    }
  }
  return ResultCode::ERR_AUDIO_NO_MATCHING_INPUT_DEVICE;
}

ResultCode
AudioInputFactory::findDefaultInputDevice(RtAudio::Api api, RtAudio::DeviceInfo* pDeviceInfo)
{
  RtAudio audio(api);
  auto deviceIds = audio.getDeviceIds();
  if (deviceIds.empty()) {
    return ResultCode::ERR_AUDIO_NO_INPUT_DEVICES;
  }
  for (auto& deviceId : deviceIds) {
    RtAudio::DeviceInfo info = audio.getDeviceInfo(deviceId);
    if (info.isDefaultInput) {
      *pDeviceInfo = info;
      return ResultCode::OK;
    }
  }
  return ResultCode::ERR_AUDIO_NO_DEFAULT_INPUT_DEVICE;
}

void
AudioInputFactory::getInputFormat(const Config::Audio::Fields& config, const RtAudio::DeviceInfo& deviceInfo,
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
    format.channelCount = deviceInfo.inputChannels;
  }
  else {
    format.channelCount = static_cast<int>(config.channelCount);
  }
  format.bytesPerFrame = getBytesPerChannel(format.sampleFormat) * format.channelCount;
}