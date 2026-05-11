#pragma once
#include "base/SettingsBase.h"

#include "Mode.h"

class Band : public SettingsBase
{
public:
  Band(RadioSettings_BandPb& raw)
    : SettingsBase(&RadioSettings_BandPb_msg)
    , m_rawSettings(raw)
    , m_name{m_rawSettings.name, sizeof(m_rawSettings.name)}
    , m_label{m_rawSettings.label, sizeof(m_rawSettings.label)}
  {
  }

  StringRef& name() { return m_name; }
  StringRef& label() { return m_label; }

  [[nodiscard]] int64_t lowestFrequency() const { return m_rawSettings.lowest_frequency; }
  [[nodiscard]] int64_t highestFrequency() const { return m_rawSettings.highest_frequency; }
  [[nodiscard]] int64_t landingFrequency() const { return m_rawSettings.landing_frequency; }
  [[nodiscard]] int32_t defaultFineStep() const { return m_rawSettings.default_fine_step; }
  [[nodiscard]] int32_t defaultCoarseStep() const { return m_rawSettings.default_coarse_step; }
  [[nodiscard]] Mode::Type defaultMode() const { return static_cast<Mode::Type>(m_rawSettings.default_mode); }

protected:
  RadioSettings_BandPb& m_rawSettings;
  StringRef m_name;
  StringRef m_label;
};
