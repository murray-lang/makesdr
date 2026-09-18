#pragma once

#include <CrossPlatformTypes.h>
#include <settings/control/sink/SettingsControlSinkT.h>
#include <usb/host/UsbHidHost.h>
#include <settings/model/radios/IRadioSettings.h>
#include <config/struct/FunCubeConfig.h>
#include "FCDHidCmd.h"
#include <cmath>

class FieldUpdate;

#define FCDPROPLUS_VENDOR_ID    0x04d8
#define FCDPROPLUS_PRODUCT_ID   0xfb31

template <typename RadioSettingsT>
class FunCubeDongleT : public SettingsControlSinkT<RadioSettingsT>, public FieldUpdateSink
{
public:
  FunCubeDongleT()
  : m_hid(FCDPROPLUS_VENDOR_ID, FCDPROPLUS_PRODUCT_ID)
  , m_lastRfGain(0.0)
  , m_lastIfGain(0.0)
  {

  }

  FunCubeDongleT(FunCubeDongleT&& rhs) noexcept
    : m_hid(::move(rhs.m_hid))
    , m_lastRfGain(rhs.m_lastRfGain)
    , m_lastIfGain(rhs.m_lastIfGain)
  {

  }
  ~FunCubeDongleT() override = default;

  FunCubeDongleT& operator=(FunCubeDongleT&& rhs) noexcept
  {
    m_hid = ::move(rhs.m_hid);
    m_lastRfGain = rhs.m_lastRfGain;
    m_lastIfGain = rhs.m_lastIfGain;
    return *this;
  }

  ResultCode applySettings(RadioSettingsT& radioSettings) override
  {
    if (!radioSettings.hasActiveBands()) {
      return ResultCode:: OK;
    }

    const typename RadioSettingsT::ActiveBandSettings* activeBandSettings = radioSettings.activeBands();
    if (!activeBandSettings->hasFocusBand()) {
      return ResultCode:: OK;
    }
    const typename RadioSettingsT::BandSettings* bandSettings = activeBandSettings->focusBandSettings();
    if (bandSettings != nullptr && bandSettings->hasRfSettings()) {
      const BandRfSettings* rfSettings = bandSettings->rfSettings();
      if (rfSettings->hasFrequency()) {
        int64_t centreFrequency = rfSettings->frequency();
        setFrequency(centreFrequency);
        setRfFilter(centreFrequency);
      }
      if (rfSettings->hasGain()) {
        float gain = rfSettings->gain();
        setLnaGain(gain);
        m_lastRfGain = gain;
      }
    }
    if (bandSettings != nullptr && bandSettings->hasIfSettings()) {
      const IfSettings* ifSettings = bandSettings->ifSettings();
      if (ifSettings->hasBandwidth()) {
        setIfFilter(ifSettings->bandwidth());
      }
      if (ifSettings->hasGain()) {
        float gain = ifSettings->gain();
        setIfGain(gain);
        m_lastIfGain = gain;
      }
    }
    return ResultCode::OK;
  }
  ResultCode applyFieldUpdate(const FieldUpdate& update) override { return ResultCode::OK; }

  ResultCode ptt(bool on) override
  {
    if (on) {
      setLnaGain(-100.0); // Mute LNA when transmitting
      setIfGain(0.0f);   // Mute IF when transmitting
    } else {
      setLnaGain(m_lastRfGain);
      setIfGain(m_lastRfGain);
    }
    return ResultCode::OK;
  }

  // void initialise(JsonVariantConst json) override;
  ResultCode configure(const Config::FunCube::Fields& config) { return m_hid.initialise(); }
  bool discover() override { return m_hid.discover(); }
  ResultCode open() override { return m_hid.open(); }
  void close() override { m_hid.close(); }
  void exit() override { m_hid.exit(); }

protected:
  ResultCode transactReport(uint8_t buf[65])
  {
    size_t bytesRW;
    ResultCode rc = m_hid.write(buf, 65, &bytesRW);
    if (rc != ResultCode::OK) return rc;
    buf[1] = 0;
    return m_hid.read(buf, 65, &bytesRW);
  }

  ResultCode setFrequency(uint32_t freqHz)
  {
    uint8_t buf[65] = {
            0,
            FCD_HID_CMD_SET_FREQUENCY_HZ,
            (uint8_t)(freqHz & 0xff),
            (uint8_t)((freqHz>>8) & 0xff),
            (uint8_t)((freqHz>>16) & 0xff),
            (uint8_t)((freqHz>>24) & 0xff)
    };
    transactReport(buf);
    if (buf[0] != FCD_HID_CMD_SET_FREQUENCY_HZ || buf[1]!=1) {
       return ResultCode::ERR_SETTING_CONTROL_SET_RF_FREQUENCY;
    }
    auto result = (uint32_t) buf[2];
    result += (uint32_t) (buf[3] << 8);
    result += (uint32_t) (buf[4] << 16);
    result += (uint32_t) (buf[5] << 24);
    if (result != freqHz) {
      return ResultCode::ERR_SETTING_CONTROL_SET_RF_FREQUENCY;
    }
    return ResultCode::OK;
  }

  ResultCode setRfFilter(TUNERRFFILTERENUM eFilter)
  {
    uint8_t buf[65] = {
      0,
      FCD_HID_CMD_SET_RF_FILTER,
      (uint8_t)eFilter
    };
    ResultCode rc = transactReport(buf);
    if (rc != ResultCode::OK) return rc;
    if(buf[0] != FCD_HID_CMD_SET_RF_FILTER) {
      return ResultCode::ERR_SETTING_CONTROL_SET_RF_FILTER;
    }
    return ResultCode::OK;
  }

  ResultCode setRfFilter(uint32_t freqHz)
  {
    TUNERRFFILTERENUM filter;
    if (freqHz < 4000000) {
      filter = TRFE_0_4;
    } else if (freqHz < 8000000) {
      filter = TRFE_4_8;
    } else if (freqHz < 16000000) {
      filter = TRFE_8_16;
    } else if (freqHz < 32000000) {
      filter = TRFE_16_32;
    } else if (freqHz < 75000000) {
      filter = TRFE_32_75;
    } else if (freqHz < 125000000) {
      filter = TRFE_75_125;
    } else if (freqHz < 148000000) {
      filter = TRFE_145;
    } else if (freqHz < 250000000) {
      filter = TRFE_125_250;
    } else if (freqHz < 438000000) {
      filter = TRFE_435;
    } else if (freqHz < 875000000) {
      filter = TRFE_410_875;
    } else {
      filter = TRFE_875_2000;
    }
    return setRfFilter(filter);
  }
  ResultCode setIfFilter(TUNERIFFILTERENUM eFilter)
  {
    uint8_t buf[65] = {
      0,
      FCD_HID_CMD_SET_IF_FILTER,
      (uint8_t)eFilter
    };
    ResultCode rc = transactReport(buf);
    if (rc != ResultCode::OK) return rc;
    if(buf[0] != FCD_HID_CMD_SET_IF_FILTER) {
      return ResultCode::ERR_SETTING_CONTROL_SET_IF_FILTER;
    }
    return ResultCode::OK;
  }

  ResultCode setIfFilter(uint32_t bandwidthHz)
  {
    TUNERIFFILTERENUM filter;
    if (bandwidthHz <= 200000) {
      filter = TIFE_200KHZ;
    } else if (bandwidthHz <= 300000) {
      filter = TIFE_300KHZ;
    } else if (bandwidthHz <= 600000) {
      filter = TIFE_600KHZ;
    } else if (bandwidthHz <= 1536000) {
      filter = TIFE_1536KHZ;
    } else if (bandwidthHz <= 5000000) {
      filter = TIFE_5MHZ;
    } else if (bandwidthHz <= 6000000) {
      filter = TIFE_6MHZ;
    } else if (bandwidthHz <= 7000000) {
      filter = TIFE_7MHZ;
    } else {
      filter = TIFE_8MHZ;
    }
    return setIfFilter(filter);
  }

  ResultCode setLnaGain(float gain)
  {
    uint8_t g;

    /* convert to nearest discrete value */
    if(gain > 27.5) {
      g = 14;              // 30.0 dB
    }
    else if(gain > 22.5) {
      g = 13;              // 25.0 dB
    }
    else if(gain > 18.75) {
      g = 12;              // 20.0 dB
    }
    else if(gain > 16.25) {
      g = 11;              // 17.5 dB
    }
    else if(gain > 13.75) {
      g = 10;              // 15.0 dB
    }
    else if(gain > 11.25) {
      g = 9;               // 12.5 dB
    }
    else if(gain > 8.75) {
      g = 8;               // 10.0 dB
    }
    else if(gain > 6.25) {
      g = 7;               // 7.5 dB
    }
    else if(gain > 3.75) {
      g = 6;               // 5.0 dB
    }
    else if(gain > 1.25) {
      g = 5;               // 2.5 dB
    }
    else if(gain > -1.25) {
      g = 4;               // 0.0 dB
    }
    else if(gain > -3.75) {
      g = 1;               // -2.5 dB
    }
    else {
      g = 0;               // -5.0 dB
    }
    uint8_t buf[65] = {
      0,
      FCD_HID_CMD_SET_LNA_GAIN,
      g
    };
    ResultCode rc =transactReport(buf);
    if (rc != ResultCode::OK) return rc;
    if(buf[0] != FCD_HID_CMD_SET_LNA_GAIN) {
      return ResultCode::ERR_SETTING_CONTROL_SET_RF_GAIN;
    }
    return ResultCode::OK;
  }

  ResultCode setIfGain(uint8_t ifGain)
  {
    uint8_t buf[65] = {
      0,
      FCD_HID_CMD_SET_IF_GAIN,
      ifGain
    };
    ResultCode rc =transactReport(buf);
    if (rc != ResultCode::OK) return rc;
    if(buf[0] != FCD_HID_CMD_SET_IF_GAIN) {
      return ResultCode::ERR_SETTING_CONTROL_SET_IF_GAIN;
    }
    return ResultCode::OK;
  }

  ResultCode setIfGain(float ifGain)
  {
    return setIfGain((uint8_t) std::lround(ifGain));
  }

protected:
  UsbHidHost m_hid;
  float m_lastRfGain;
  float m_lastIfGain;
  
};
