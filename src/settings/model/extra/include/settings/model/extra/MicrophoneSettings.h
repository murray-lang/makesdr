#pragma once
#include "SettingsBase.h"
#include "SteppableFloatSetting.h"

class MicrophoneSettings : public SettingsBase
{
public:
  MicrophoneSettings(makesdr_MicrophoneSettingsPb& mic)
  : m_rawSettings(mic)
  , m_gain(mic.gain) {}

  SteppableFloatSetting& gain() { return m_gain; }
  [[nodiscard]] const SteppableFloatSetting& gain() const { return m_gain; }
protected:
  makesdr_MicrophoneSettingsPb& m_rawSettings;
  SteppableFloatSetting m_gain;
};
