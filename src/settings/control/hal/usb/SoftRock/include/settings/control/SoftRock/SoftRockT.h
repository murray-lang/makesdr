#pragma once

#include <settings/control/sink/SettingsControlSinkT.h>
#include <usb/host/UsbHost.h>
#include <settings/model/radios/IRadioSettings.h>
#include <config/struct/SoftRockConfig.h>
#include <cmath>

#define SOFTROCK_CMD_BUFFER_SIZE 4

#define SOFTROCK_VENDOR_ID    0x16C0
#define SOFTROCK_PRODUCT_ID   0x05DC

class FieldUpdate;

template <typename RadioSettingsT>
class SoftRockT : public SettingsControlSinkT<RadioSettingsT>, public FieldUpdateSink
{
public:
  enum class Request
  {
    FIRMWARE_VERSION = 0x00,
    SET_FREQUENCY = 0x32,
    GET_FREQUENCY = 0x3A,
    PTT = 0x50
  };

  SoftRockT()
    : m_usbHost(SOFTROCK_VENDOR_ID, SOFTROCK_PRODUCT_ID)
    , m_cmdBuffer{0}
  {

  }
  SoftRockT(SoftRockT&& rhs) noexcept
    : m_usbHost(::move(rhs.m_usbHost))
    , m_cmdBuffer{0}
  {

  }
  ~SoftRockT() override = default;

  SoftRockT& operator=(SoftRockT&& rhs) noexcept
  {
    m_usbHost = ::move(rhs.m_usbHost);
    return *this;
  }

  // [[nodiscard]] const ThreadRequirements* getThreadRequirements() const override;
  // bool tick() override;

  // void applySettings(const RadioSettings& radioSettings) override;
  // void readSettings(RadioSettings& radioSettings) override;
  // void applySettings(const RadioSettings& radioSettings, BandSettings* pBandSettings) override;
  ResultCode applySettings(RadioSettingsT& radioSettings) override
  {
    if (!radioSettings.hasActiveBands()) {
      return ResultCode:: OK;
    }

    const typename RadioSettingsT::ActiveBandSettings* activeBandSettings = radioSettings.activeBands();
    if (activeBandSettings == nullptr || !activeBandSettings->hasFocusBand()) {
      return ResultCode:: OK;
    }
    const typename RadioSettingsT::BandSettings* bandSettings = activeBandSettings->focusBandSettings();
    if (bandSettings != nullptr && bandSettings->hasRfSettings()) {
      const BandRfSettings* rfSettings = bandSettings->rfSettings();
      if (rfSettings != nullptr && rfSettings->hasFrequency()) {
        int64_t centreFrequency = rfSettings->frequency();
        setFrequency(centreFrequency);
      }
    }
    return ResultCode::OK;
  }

  ResultCode applyFieldUpdate(const FieldUpdate& update) override
  {
    return ResultCode::OK; //ignore for now.
  }

  ResultCode ptt(bool on) override
  {
    m_usbHost.write(static_cast<uint8_t>(Request::PTT), on ? 1 : 0, 0);
    return ResultCode::OK;
  }

  // void initialise(JsonVariantConst json) override;
  ResultCode configure(const Config::SoftRock::Fields& config) { return m_usbHost.initialise(); }
  bool discover() override { return m_usbHost.discover(); }
  ResultCode open() override { return m_usbHost.open(); }
  void close() override { m_usbHost.close(); }
  void exit() override { m_usbHost.exit(); }

protected:
  ResultCode setFrequency(uint32_t freqHz)
  {
    packFrequency(freqHz, m_cmdBuffer);
    size_t written;
    return m_usbHost.write(static_cast<uint8_t>(Request::SET_FREQUENCY), 0, 0, m_cmdBuffer, sizeof(m_cmdBuffer), &written);
  }

  static void packFrequency(uint32_t freqHz, uint8_t *out_buffer)
  {
    // 1. Calculate Si570 output frequency in MHz (RF frequency * 4)
    double freqx4 = (static_cast<double>(freqHz) * 4.0) / 1000000.0;

    // 2. Convert to 11.21 fixed point format
    auto fixed_point_freq = static_cast<uint32_t>(freqx4 * static_cast<double>(1 << 21));

    // 3. Pack into 4-byte array (Little Endian format)
    out_buffer[0] = static_cast<uint8_t>(fixed_point_freq & 0xFF);
    out_buffer[1] = static_cast<uint8_t>((fixed_point_freq >> 8) & 0xFF);
    out_buffer[2] = static_cast<uint8_t>((fixed_point_freq >> 16) & 0xFF);
    out_buffer[3] = static_cast<uint8_t>((fixed_point_freq >> 24) & 0xFF);
  }

protected:
  UsbHost m_usbHost;
  uint8_t m_cmdBuffer[SOFTROCK_CMD_BUFFER_SIZE];
};
