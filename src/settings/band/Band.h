#pragma once
#include "../base/SettingsBase.h"

#include "../mode/Mode.h"

class Band
{
public:
  Band(RadioSettings_BandPb& raw)
    : m_rawSettings(raw)
    , m_name{raw.name, raw.name, sizeof(raw.name)}
    , m_label{raw.label, raw.label, sizeof(m_rawSettings.label)}
  {
  }

  [[nodiscard]] const StringRef& name() const { return m_name; }
  [[nodiscard]] const StringRef& label() const { return m_label; }

  [[nodiscard]] int64_t lowestFrequency() const { return m_rawSettings.lowest_frequency; }
  [[nodiscard]] int64_t highestFrequency() const { return m_rawSettings.highest_frequency; }
  [[nodiscard]] int64_t landingFrequency() const { return m_rawSettings.landing_frequency; }
  [[nodiscard]] int32_t defaultFineStep() const { return m_rawSettings.default_fine_step; }
  [[nodiscard]] int32_t defaultCoarseStep() const { return m_rawSettings.default_coarse_step; }
  [[nodiscard]] Mode::Type defaultMode() const { return static_cast<Mode::Type>(m_rawSettings.default_mode); }

  RadioSettings_BandPb& raw() { return m_rawSettings; }
  [[nodiscard]] const RadioSettings_BandPb& raw() const { return m_rawSettings; }

protected:

  RadioSettings_BandPb& m_rawSettings;
  const StringRef m_name;
  const StringRef m_label;
};
