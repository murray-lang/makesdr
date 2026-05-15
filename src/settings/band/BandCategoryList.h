#pragma once

#include "BandCategory.h"

using BandCategoryVector = etl::vector<BandCategory, 8>;

class BandCategoryList
{
public:
  BandCategoryList(RadioSettings_BandCategoryListPb& raw)
    : m_rawSettings(raw)
  {
    uint32_t max = count();
    for (uint32_t i = 0; i < max; i++) {
      m_categories.emplace_back(m_rawSettings.categories[i]);
    }
  }

  [[nodiscard]] uint32_t count() const { return m_rawSettings.categories_count; }

  const BandCategory* findCategory(const char* name) const
  {
    uint32_t max = count();
    for (uint32_t i = 0; i < max; i++) {
      if (m_categories[i].name() == name) {
        return &m_categories[i];
      }
    }
    return nullptr;
  }

  const Band* findBand(const char* name) const
  {
    uint32_t max = count();
    for (uint32_t i = 0; i < max; i++) {
      if (m_categories[i].findBand(name)) {
        return m_categories[i].findBand(name);
      }
    }
    return nullptr;
  }

protected:

  RadioSettings_BandCategoryListPb& m_rawSettings;
  BandCategoryVector m_categories;
};