#pragma once
#include "Band.h"
#include <etl/vector.h>

using BandVector = etl::vector<Band, 34>;


class BandList
{
public:
  BandList(RadioSettings_BandListPb& raw);

  [[nodiscard]] uint32_t count() const { return m_rawSettings.bands_count; }

  const Band* findBand(const char* name) const;
  const BandVector& bands() const { return m_bands; }

protected:

  const RadioSettings_BandListPb& m_rawSettings;
  BandVector m_bands;
};