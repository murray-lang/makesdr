#pragma once
#include "settings/model/proto/RadioSettings.pb.h"
#include "BandList.h"

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

  BandCategory(const BandCategory& rhs) : BandCategory(rhs.m_rawSettings) {}

  // BandCategory(const BandCategory& rhs)
  //   : m_rawSettings(rhs.m_rawSettings)
  //   , m_name(rhs.m_rawSettings.name, rhs.m_rawSettings.name, sizeof(rhs.m_rawSettings.name))
  //   , m_label(rhs.m_rawSettings.label, rhs.m_rawSettings.label, sizeof(rhs.m_rawSettings.label))
  //   , m_bands(rhs.m_bands)
  // {}
  // BandCategory(BandCategory&& rhs) noexcept = delete;

  // BandCategory& operator=(const BandCategory& rhs) noexcept
  // {
  //
  // }

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