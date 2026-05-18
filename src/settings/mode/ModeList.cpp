#include "ModeList.h"

#include "util/PbUtils.h"

ModeList::ModeList(const RadioSettings_ModeListPb& raw)
   : m_rawSettings(raw)
{
  for (uint32_t i = 0; i < m_rawSettings.modes_count; i++) {
    m_modes.emplace_back( m_rawSettings.modes[i]);
  }
}

const Mode*
ModeList::findModeByType(Mode::Type type) const {
  for (const auto& mode : m_modes) {
    if (mode.type() == type) {
      return &mode;
    }
  }
  return nullptr;
}

ResultCode
ModeList::readProtobuf(const uint8_t *buffer, size_t msg_length) {
  return PbUtils::readProtobuf<RadioSettings_ModeListPb>(
    buffer,
    msg_length,
    RadioSettings_ModeListPb_fields,
    RadioSettings_ModeListPb_init_zero,
    m_rawSettings
  );
}