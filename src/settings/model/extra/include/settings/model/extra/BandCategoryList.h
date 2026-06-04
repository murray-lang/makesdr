#pragma once

#include "BandCategory.h"
#include <ResultCode.h>

using BandCategoryVector = etl::vector<BandCategory, MAX_BAND_CATEGORIES>;

class BandCategoryList
{
public:
  BandCategoryList(RadioSettings_BandCategoryListPb& raw);

  [[nodiscard]] uint32_t count() const { return m_rawSettings.categories_count; }

  const BandCategory* findCategory(const char* name) const;
  const Band* findBand(const char* name) const;

  ResultCode readProtobuf(const uint8_t *buffer, size_t msg_length);

protected:

  RadioSettings_BandCategoryListPb& m_rawSettings;
  BandCategoryVector m_categories;
};