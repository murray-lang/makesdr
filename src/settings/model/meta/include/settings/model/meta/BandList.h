#pragma once
#include <CrossPlatformTypes.h>
#include <RadioSettings.pb.h>

#include "types.h"


#ifdef USE_ETL
#include <etl/span.h>
using BandSpan = etl::span<const RadioSettings_BandPb, MAX_BANDS_PER_CATEGORY>;
#else
using BandSpan = std::span<const RadioSettings_BandPb>;
#endif


class BandList
{
public:
  BandList(const RadioSettings_BandListPb& raw);

  BandList(BandList&&)  noexcept = default;
  // BandList& operator=(BandList&&) noexcept = default;

  [[nodiscard]] uint32_t count() const { return m_rawSettings.bands_count; }

  const RadioSettings_BandPb* findBand(const BandName& name) const;
  const BandSpan& bands() const { return m_bands; }

protected:

  const RadioSettings_BandListPb& m_rawSettings;
  BandSpan m_bands;
};