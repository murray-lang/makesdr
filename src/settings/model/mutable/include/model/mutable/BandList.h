#pragma once
#include <CrossPlatformTypes.h>
#include "model/basic/Band.h"
#include <etl/vector.h>

#ifdef USE_ETL
using BandVector = etl::vector<Band, MAX_BANDS_PER_CATEGORY>;
#else
using BandVector = std::vector<Band>;
#endif


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