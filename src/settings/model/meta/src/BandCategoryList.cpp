#include "settings/model/meta/BandCategoryList.h"

#include "settings/model/meta/BandList.h"

// #include "settings/model/core/ProtobufIo.h"

BandCategoryList::BandCategoryList(const makesdr_BandCategoryListPb& raw)
  : m_rawSettings(raw)
  , m_categories(raw.categories, MAX_BAND_CATEGORIES)
{
}

const makesdr_BandCategoryPb*
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

const makesdr_BandPb*
BandCategoryList::findBand(const BandName& name) const
{
  uint32_t max = count();
  for (uint32_t i = 0; i < max; i++) {
    BandList bands(m_categories.at(i).bands);
    const makesdr_BandPb* band = bands.findBand(name);
    if (band != nullptr) {
      return band;
    }
  }
  return nullptr;
}

// ResultCode
// BandCategoryList::readProtobuf(const uint8_t *buffer, size_t msg_length) {
//   return ProtobufIo::readProtobuf<makesdr_BandCategoryListPb>(
//     buffer,
//     msg_length,
//     makesdr_BandCategoryListPb_fields,
//     makesdr_BandCategoryListPb_init_zero,
//     m_rawSettings
//   );
// }