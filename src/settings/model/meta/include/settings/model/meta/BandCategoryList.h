#pragma once

#include <ResultCode.h>
#include "RadioSettings.pb.h"
#include "types.h"


#ifdef USE_ETL
#include <etl/span.h>

using BandCategorySpan = etl::span<const RadioSettings_BandCategoryPb, MAX_BAND_CATEGORIES>;
#else
using BandCategorySpan = etl::span<const RadioSettings_BandCategoryPb>;
#endif

class BandCategoryList
{
public:
  BandCategoryList(const RadioSettings_BandCategoryListPb& raw);

  [[nodiscard]] uint32_t count() const { return m_rawSettings.categories_count; }

  const RadioSettings_BandCategoryPb* findCategory(const BandCategoryName& name) const;
  const RadioSettings_BandPb* findBand(const BandName& name) const;

  // ResultCode readProtobuf(const uint8_t *buffer, size_t msg_length);

protected:

  const RadioSettings_BandCategoryListPb& m_rawSettings;
  BandCategorySpan m_categories;
};