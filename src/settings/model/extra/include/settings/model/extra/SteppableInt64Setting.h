#pragma once
#include "SettingsBase.h"

class SteppableInt64Setting : public SettingsBase
{
public:
  SteppableInt64Setting(makesdr_SteppableInt64SettingPb& raw)
    : m_rawSettings(raw)
  {}

  [[nodiscard]] int64_t value() const { return m_rawSettings.value; }
  [[nodiscard]] int64_t coarseDelta() const { return m_rawSettings.coarse_delta; }
  [[nodiscard]] int64_t fineDelta() const { return m_rawSettings.fine_delta; }
  [[nodiscard]] bool useFine() const { return m_rawSettings.use_fine; }
protected:
  makesdr_SteppableInt64SettingPb& m_rawSettings;
};
