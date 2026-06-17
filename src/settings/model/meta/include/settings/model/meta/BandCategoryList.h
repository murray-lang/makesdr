#pragma once

#include <ResultCode.h>
#include "settings/model/proto/RadioSettings.pb.h"
#include "types.h"


#ifdef USE_ETL
#include <etl/span.h>

using BandCategorySpan = etl::span<const makesdr_BandCategoryPb, MAX_BAND_CATEGORIES>;
#else
using BandCategorySpan = etl::span<const makesdr_BandCategoryPb>;
#endif

class BandCategoryList
{
public:
  BandCategoryList(const makesdr_BandCategoryListPb& raw);

  [[nodiscard]] uint32_t count() const { return m_rawSettings.categories_count; }

  const makesdr_BandCategoryPb* findCategory(const BandCategoryName& name) const;
  const makesdr_BandPb* findBand(const BandName& name) const;

  // ResultCode readProtobuf(const uint8_t *buffer, size_t msg_length);

protected:

  const makesdr_BandCategoryListPb& m_rawSettings;
  BandCategorySpan m_categories;
};