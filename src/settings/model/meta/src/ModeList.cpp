#include "settings/model/meta/ModeList.h"

ModeList::ModeList(const RadioSettings_ModeListPb& raw)
   : m_rawSettings(raw)
   , m_modes(raw.modes, MAX_MODES)
{
}

const RadioSettings_ModePb*
ModeList::findModeByType(RadioSettings_ModeType type) const {
  for (const auto& mode : m_modes) {
    if (mode.type == type) {
      return &mode;
    }
  }
  return nullptr;
}
