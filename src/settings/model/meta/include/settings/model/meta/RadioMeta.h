#pragma once
#include "BandCategoryList.h"
#include "ModeList.h"

class RadioMeta
{
public:
  RadioMeta(const RadioSettings_RadioMetaPb& raw)
    : m_rawSettings(raw)
    , m_bands(raw.bands)
    , m_modes(raw.modes)
  {}

  RadioMeta(const RadioMeta& rhs) : RadioMeta(rhs.m_rawSettings) {}
  // RadioCategories& operator=(const RadioCategories&) = delete;

  // Allow moving if needed
  RadioMeta(RadioMeta&&) = default;
  // RadioMeta& operator=(RadioMeta&&) = default;

  const RadioSettings_RadioMetaPb& raw() const { return m_rawSettings; }
  
  bool hasBands() const { return m_rawSettings.has_bands; }
  bool hasModes() const { return m_rawSettings.has_modes; }

  BandCategoryList& bands() { return m_bands; }
  [[nodiscard]] const BandCategoryList& bands() const { return m_bands; }
  ModeList& modes() { return m_modes; }
  [[nodiscard]] const ModeList& modes() const { return m_modes; }

protected:
  const RadioSettings_RadioMetaPb& m_rawSettings;
  BandCategoryList m_bands;
  ModeList m_modes;
};
