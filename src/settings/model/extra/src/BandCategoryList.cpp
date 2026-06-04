#include "settings/model/extra/BandCategoryList.h"

#include "settings/model/core/ProtobufIo.h"

BandCategoryList::BandCategoryList(RadioSettings_BandCategoryListPb& raw)
   : m_rawSettings(raw)
{
  uint32_t max = count();
  for (uint32_t i = 0; i < max; i++) {
    m_categories.emplace_back(m_rawSettings.categories[i]);
  }
}

const BandCategory*
BandCategoryList::findCategory(const char* name) const
{
  uint32_t max = count();
  for (uint32_t i = 0; i < max; i++) {
    if (m_categories[i].name() == name) {
      return &m_categories[i];
    }
  }
  return nullptr;
}

const Band*
BandCategoryList::findBand(const char* name) const
{
  uint32_t max = count();
  for (uint32_t i = 0; i < max; i++) {
    if (m_categories[i].findBand(name)) {
      return m_categories[i].findBand(name);
    }
  }
  return nullptr;
}

ResultCode
BandCategoryList::readProtobuf(const uint8_t *buffer, size_t msg_length) {
  return ProtobufIo::readProtobuf<RadioSettings_BandCategoryListPb>(
    buffer,
    msg_length,
    RadioSettings_BandCategoryListPb_fields,
    RadioSettings_BandCategoryListPb_init_zero,
    m_rawSettings
  );
}