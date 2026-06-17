#include "settings/model/meta/ModeList.h"

ModeList::ModeList(const makesdr_ModeListPb& raw)
   : m_rawSettings(raw)
   , m_modes(raw.modes, MAX_MODES)
{
}

const makesdr_ModePb*
ModeList::findModeByType(makesdr_ModeType type) const {
  for (const auto& mode : m_modes) {
    if (mode.type == type) {
      return &mode;
    }
  }
  return nullptr;
}
