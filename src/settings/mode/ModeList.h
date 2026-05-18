#pragma once
#include "Mode.h"
#include "../base/SettingsBase.h"
#include <etl/vector.h>

#define MAX_MODE_COUNT 10

using ModeVector = etl::vector<Mode, MAX_MODE_COUNT>;

class ModeList : public SettingsBase
{
public:
  ModeList(const RadioSettings_ModeListPb& raw);

  const ModeVector& modes() const { return m_modes; }

  const Mode* findModeByType(Mode::Type type) const;

  ResultCode readProtobuf(const uint8_t *buffer, size_t msg_length);

protected:
  RadioSettings_ModeListPb m_rawSettings;
  ModeVector m_modes;
};