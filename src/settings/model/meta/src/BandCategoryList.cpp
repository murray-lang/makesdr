#include "settings/model/meta/BandCategoryList.h"

#include "settings/model/meta/BandList.h"

// #include "settings/model/core/ProtobufIo.h"

BandCategoryList::BandCategoryList(const RadioSettings_BandCategoryListPb& raw)
  : m_rawSettings(raw)
  , m_categories(raw.categories, MAX_BAND_CATEGORIES)
{
}

const RadioSettings_BandCategoryPb*
BandCategoryList::findCategory(const BandCategoryName& name) const
{
  uint32_t max = count();
  for (uint32_t i = 0; i < max; i++) {
    if (name == m_categories.at(i).name) {
      return &m_categories.at(i);
    }
  }
  return nullptr;
}

const RadioSettings_BandPb*
BandCategoryList::findBand(const BandName& name) const
{
  uint32_t max = count();
  for (uint32_t i = 0; i < max; i++) {
    BandList bands(m_categories.at(i).bands);
    const RadioSettings_BandPb* band = bands.findBand(name);
    if (band != nullptr) {
      return band;
    }
  }
  return nullptr;
}

// ResultCode
// BandCategoryList::readProtobuf(const uint8_t *buffer, size_t msg_length) {
//   return ProtobufIo::readProtobuf<RadioSettings_BandCategoryListPb>(
//     buffer,
//     msg_length,
//     RadioSettings_BandCategoryListPb_fields,
//     RadioSettings_BandCategoryListPb_init_zero,
//     m_rawSettings
//   );
// }