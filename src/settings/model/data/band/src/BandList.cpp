#include "settings/model/data/band/BandList.h"
#include <etl/string.h>

BandList::BandList(const makesdr_BandListPb& raw)
  : m_rawSettings(raw)
  , m_bands(raw.bands, raw.bands_count)
{

}

const makesdr_BandPb*
BandList::findBand(const BandName& name) const
{
  uint32_t max = count();
  for (uint32_t i = 0; i < max; i++) {
    if (name == m_bands[i].name) {
      return &m_bands[i];
    }
  }
  return nullptr;
}