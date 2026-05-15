#pragma once
#include "../base/SettingsBase.h"
#include "../base/SteppableFloatSetting.h"

class MicrophoneSettings : public SettingsBase
{
public:
  MicrophoneSettings(RadioSettings_MicrophoneSettingsPb& mic)
  : SettingsBase(&RadioSettings_MicrophoneSettingsPb_msg)
  , m_rawSettings(mic)
  , m_gain(mic.gain) {}

  SteppableFloatSetting& gain() { return m_gain; }
  [[nodiscard]] const SteppableFloatSetting& gain() const { return m_gain; }
protected:
  void* getMessage() override { return &m_rawSettings; }

  RadioSettings_MicrophoneSettingsPb& m_rawSettings;
  SteppableFloatSetting m_gain;
};
