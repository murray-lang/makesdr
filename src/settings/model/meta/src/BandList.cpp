#include <settings/model/meta/BandList.h>

#include "etl/string.h"

BandList::BandList(const RadioSettings_BandListPb& raw)
  : m_rawSettings(raw)
  , m_bands(raw.bands, MAX_BANDS_PER_CATEGORY)
{

}

const RadioSettings_BandPb*
BandList::findBand(const BandName& name) const
{
  uint32_t max = count();
  for (uint32_t i = 0; i < max; i++) {
    if (name == m_bands[i].name) {
      return &m_bands.at(i);
    }
  }
  return nullptr;
}