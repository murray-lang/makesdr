#ifndef LINUX_AUDIO_DRIVER_H
#define LINUX_AUDIO_DRIVER_H
#include <rtaudio/RtAudio.h>
#include <optional>

class RtAudioDriver
{
protected:
  ~RtAudioDriver() = default;

public:

  RtAudioDriver() = default;
  RtAudioDriver(RtAudio::Api api, const RtAudio::DeviceInfo& deviceInfo)
  {
    bindApi(api, deviceInfo);
  }
  // RtAudioDriver(RtAudioDriver&&) = default;
  // RtAudioDriver& operator=(RtAudioDriver&&) = default;
  RtAudioDriver(const RtAudioDriver&)            = delete;
  RtAudioDriver& operator=(const RtAudioDriver&) = delete;
  RtAudioDriver(RtAudioDriver&&)                 = delete;
  RtAudioDriver& operator=(RtAudioDriver&&)      = delete;

protected:
  // DeviceInfo::ID is only meaningful within the API it was enumerated from, so
  // the stream has to be opened on that same API rather than RtAudio's default.
  void bindApi(RtAudio::Api api, const RtAudio::DeviceInfo& deviceInfo)
  {
    m_rtAudio.emplace(api);
    m_deviceInfo = deviceInfo;
    // Probe here, at configure time, while none of our streams are open.
    // RtApiAlsa::probeDeviceInfo() opens a device's playback side first and
    // bails out on EBUSY, so a card that anything is already playing to
    // vanishes from every later probe - even when all we want is its capture
    // side, and even though ALSA would happily give us that side. Our own
    // config does exactly this: the transmitter drives output and IQ capture
    // through the same card. Once a device is in this instance's list it
    // survives subsequent probes, so an ID cached now outlives those opens.
    m_deviceIdValid = lookUpDeviceId(m_deviceId);
  }

  [[nodiscard]] bool isApiBound() const { return m_rtAudio.has_value(); }

  // Hands back the ID to open the stream with. DeviceInfo::ID must never be
  // used for that: it comes from a counter owned by whichever RtApi instance
  // probed the device, and that counter only advances for devices that probed
  // successfully, so an ID captured by the enumerating RtAudio can name a
  // different device - or none at all - in the instance we open with, and
  // openStream() then fails with RTAUDIO_INVALID_PARAMETER.
  [[nodiscard]] bool resolveDeviceId(unsigned int& deviceId)
  {
    // A device absent at bind time may have been plugged in since.
    if (!m_deviceIdValid) {
      m_deviceIdValid = lookUpDeviceId(m_deviceId);
    }
    deviceId = m_deviceId;
    return m_deviceIdValid;
  }

  // Deferred so the API can be chosen at configure time; empty until bound.
  std::optional<RtAudio> m_rtAudio;
  RtAudio::DeviceInfo m_deviceInfo;

private:
  // Not const: RtAudio's enumeration accessors re-probe, so they are non-const.
  bool lookUpDeviceId(unsigned int& deviceId)
  {
    if (!m_rtAudio) {
      return false;
    }
    for (unsigned int id : m_rtAudio->getDeviceIds()) {
      if (m_rtAudio->getDeviceInfo(id).name == m_deviceInfo.name) {
        deviceId = id;
        return true;
      }
    }
    return false;
  }

  unsigned int m_deviceId = 0;
  bool m_deviceIdValid = false;
};

#endif //LINUX_AUDIO_DRIVER_H
