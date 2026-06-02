#pragma once
#include "BandCategoryList.h"
#include "../../../../core/include/model/core/SettingsBase.h"
#include "ModeList.h"

class RadioCategories : public SettingsBase
{
public:
  RadioCategories(RadioSettings_RadioCategoriesPb& raw)
    : m_rawSettings(raw)
    , m_bands(raw.bands)
    , m_modes(raw.modes)
  {}

  RadioCategories(const RadioCategories& rhs) : RadioCategories(rhs.m_rawSettings) {}
  // RadioCategories& operator=(const RadioCategories&) = delete;

  // Allow moving if needed
  RadioCategories(RadioCategories&&) = default;
  RadioCategories& operator=(RadioCategories&&) = default;

  BandCategoryList& bands() { return m_bands; }
  [[nodiscard]] const BandCategoryList& bands() const { return m_bands; }
  ModeList& modes() { return m_modes; }
  [[nodiscard]] const ModeList& modes() const { return m_modes; }

protected:
  RadioSettings_RadioCategoriesPb& m_rawSettings;
  BandCategoryList m_bands;
  ModeList m_modes;
};
