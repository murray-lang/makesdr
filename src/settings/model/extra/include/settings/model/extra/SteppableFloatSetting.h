#pragma once
#include "SettingsBase.h"

class SteppableFloatSetting : public SettingsBase
{
public:
  SteppableFloatSetting(makesdr_SteppableFloatSettingPb& raw)
    : m_rawSettings(raw)
  {}

  [[nodiscard]] float value() const { return m_rawSettings.value; }
  [[nodiscard]] float coarseDelta() const { return m_rawSettings.coarse_delta; }
  [[nodiscard]] float fineDelta() const { return m_rawSettings.fine_delta; }
  [[nodiscard]] bool useFine() const { return m_rawSettings.use_fine; }
protected:
  makesdr_SteppableFloatSettingPb& m_rawSettings;
};
