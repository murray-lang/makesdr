#pragma once
#include "Mode.h"
#include "../base/SettingsBase.h"

#define MAX_MODE_COUNT 10

using ModeVector = etl::vector<Mode, MAX_MODE_COUNT>;

class ModeList : public SettingsBase
{
public:
  ModeList(const RadioSettings_ModeListPb& raw)
    : SettingsBase(&RadioSettings_ModeListPb_msg)
    , m_rawSettings(raw)
  {
    for (uint32_t i = 0; i < m_rawSettings.modes_count; i++) {
      m_modes.emplace_back( m_rawSettings.modes[i]);
    }
  }

  const ModeVector& modes() const { return m_modes; }

  const Mode* findModeByType(Mode::Type type) const {
    for (const auto& mode : m_modes) {
      if (mode.type() == type) {
        return &mode;
      }
    }
    return nullptr;
  }

protected:
  void* getMessage() override { return &m_rawSettings; }

  RadioSettings_ModeListPb m_rawSettings;
  ModeVector m_modes;
};