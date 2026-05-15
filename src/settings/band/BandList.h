#pragma once
#include "Band.h"
#include <etl/vector.h>

using BandVector = etl::vector<Band, 34>;


class BandList
{
public:
  BandList(RadioSettings_BandListPb& raw)
    : m_rawSettings(raw)
  {
    uint32_t max = count();
    for (uint32_t i = 0; i < max; i++) {
      m_bands.emplace_back(raw.bands[i]);
    }
  }

  [[nodiscard]] uint32_t count() const { return m_rawSettings.bands_count; }

  const Band* findBand(const char* name) const
  {
    uint32_t max = count();
    for (uint32_t i = 0; i < max; i++) {
      if (m_bands[i].name() == name) {
        return &m_bands[i];
      }
    }
    return nullptr;
  }

  const BandVector& bands() const { return m_bands; }

protected:

  const RadioSettings_BandListPb& m_rawSettings;
  BandVector m_bands;
};