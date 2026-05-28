#include <model/mutable/BandList.h>

BandList::BandList(RadioSettings_BandListPb& raw)
    : m_rawSettings(raw)
{
  uint32_t max = count();
  for (uint32_t i = 0; i < max; i++) {
    m_bands.emplace_back(raw.bands[i]);
  }
}
const Band*
BandList::findBand(const char* name) const
{
  uint32_t max = count();
  for (uint32_t i = 0; i < max; i++) {
    if (m_bands[i].name() == name) {
      return &m_bands[i];
    }
  }
  return nullptr;
}