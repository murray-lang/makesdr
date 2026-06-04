#pragma once
#include "settings/model/core/SettingsBase.h"

class SteppableInt64Setting : public SettingsBase
{
public:
  SteppableInt64Setting(RadioSettings_SteppableInt64SettingPb& raw)
    : m_rawSettings(raw)
  {}

  [[nodiscard]] int64_t value() const { return m_rawSettings.value; }
  [[nodiscard]] int64_t coarseDelta() const { return m_rawSettings.coarse_delta; }
  [[nodiscard]] int64_t fineDelta() const { return m_rawSettings.fine_delta; }
  [[nodiscard]] bool useFine() const { return m_rawSettings.use_fine; }
protected:
  RadioSettings_SteppableInt64SettingPb& m_rawSettings;
};
