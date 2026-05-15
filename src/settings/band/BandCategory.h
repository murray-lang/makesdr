#pragma once
#include "BandList.h"
#include "base/SettingsBase.h"

class BandCategory
{
public:
  BandCategory(RadioSettings_BandCategoryPb& raw)
    : m_rawSettings(raw)
    , m_name{raw.name, raw.name, sizeof(raw.name)}
    , m_label{raw.label, raw.label, sizeof(raw.label)}
    , m_bands(raw.bands)
  {
  }

  [[nodiscard]] const StringRef& name() const { return m_name; }
  [[nodiscard]] const StringRef& label() const { return m_label; }

  BandList& bands() { return m_bands; }

  const Band* findBand(const char* name) const { return m_bands.findBand(name);};

protected:

  RadioSettings_BandCategoryPb& m_rawSettings;
  const StringRef m_name;
  const StringRef m_label;
  BandList m_bands;
};