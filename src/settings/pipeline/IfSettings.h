#pragma once
#include "../base/SettingsBase.h"
#include "../base/SteppableFloatSetting.h"

class IfSettings : public SettingsBase
{
public:
  IfSettings(RadioSettings_IfSettingsPb& rawSettings)
    : SettingsBase(&RadioSettings_IfSettingsPb_msg)
    , m_rawSettings(rawSettings)
    , m_gain(rawSettings.gain)
  {}

  [[nodiscard]] bool hasBandwidth() const { return m_rawSettings.has_bandwidth; }
  [[nodiscard]] bool hasGain() const { return m_rawSettings.has_gain; }

  [[nodiscard]] uint32_t bandwidth() const { return m_rawSettings.bandwidth; }
  SteppableFloatSetting& gain() { return m_gain; }
  [[nodiscard]] const SteppableFloatSetting& gain() const { return m_gain; }

protected:
  void * getMessage() override { return &m_rawSettings; }

  RadioSettings_IfSettingsPb& m_rawSettings;
  SteppableFloatSetting m_gain;
};
