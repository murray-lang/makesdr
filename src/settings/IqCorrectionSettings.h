#pragma once

#include "base/SettingsBase.h"
#include "base/SteppableFloatSetting.h"

class IqCorrectionSettings : public SettingsBase
{
public:
  IqCorrectionSettings(RadioSettings_IqCorrectionSettingsPb& raw)
    : SettingsBase(&RadioSettings_IqCorrectionSettingsPb_msg)
    , m_rawSettings(raw)
    , m_amplitude(raw.amplitude)
    , m_phase(raw.phase)
  {}

  SteppableFloatSetting& amplitude() { return m_amplitude; }
  SteppableFloatSetting& phase() { return m_phase; }
  [[nodiscard]] const SteppableFloatSetting& amplitude() const { return m_amplitude; }
  [[nodiscard]] const SteppableFloatSetting& phase() const { return m_phase; }

protected:
  RadioSettings_IqCorrectionSettingsPb& m_rawSettings;
  SteppableFloatSetting m_amplitude;
  SteppableFloatSetting m_phase;
};
