#pragma once


#include "settings/control/sink/SettingsControlSink.h"
#include "settings/control/usb/HidUsbControl.h"
#include "settings/model/core/RadioSettings.h"
#include "config/struct/FunCubeConfig.h"
#include "FCDHidCmd.h"

class SettingUpdate;

class FunCubeDongle : public SettingsControlSink, public SettingUpdateSink
{
public:
  FunCubeDongle();
  ~FunCubeDongle() override;

    // void applySettings(const RadioSettings& radioSettings) override;
    // void readSettings(RadioSettings& radioSettings) override;
  // void applySettings(const RadioSettings& radioSettings, BandSettings* pBandSettings) override;
  ResultCode applySettings(const RadioSettings& radioSettings) override;
  ResultCode applySettingUpdate(const SettingUpdate& update) override
  {
    return ResultCode::OK; //ignore for now.
  }

  void ptt(bool on) override;

  // void initialise(JsonVariantConst json) override;
  ResultCode configure(const Config::FunCube::Fields& config);
  bool discover() override;
  ResultCode open() override;
  void close() override;
  void exit() override;

protected:
  ResultCode transactReport(uint8_t buf[65]);
  ResultCode setFrequency(uint32_t freqHz);
  ResultCode setRfFilter(TUNERRFFILTERENUM eFilter);
  ResultCode setRfFilter(uint32_t freqHz);
  ResultCode setIfFilter(TUNERIFFILTERENUM eFilter);
  ResultCode setIfFilter(uint32_t bandwidthHz);
  ResultCode setLnaGain(float gain);
  ResultCode setIfGain(uint8_t ifGain);
  ResultCode setIfGain(float ifGain);

protected:
  HidUsbControl m_control;
  float m_lastRfGain;
  float m_lastIfGain;
};
