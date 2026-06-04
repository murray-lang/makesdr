#pragma once
#include <CrossPlatformTypes.h>
#include "Mode.h"
#include "settings/model/core/SettingsBase.h"

#include <ResultCode.h>

#define MAX_MODE_COUNT 10

#ifdef USE_ETL
using ModeVector = etl::vector<Mode, MAX_MODE_COUNT>;
#else
using ModeVector = std::vector<Mode>;
#endif

class ModeList : public SettingsBase
{
public:
  ModeList(const RadioSettings_ModeListPb& raw);

  // Prevent copying since we hold non-copyable Mode objects
  ModeList(const ModeList&) = delete;
  ModeList& operator=(const ModeList&) = delete;

  // Allow moving if needed
  ModeList(ModeList&&)  noexcept = default;
  ModeList& operator=(ModeList&&)  noexcept = default;

  [[nodiscard]] const ModeVector& modes() const { return m_modes; }

  [[nodiscard]] const Mode* findModeByType(Mode::Type type) const;

  ResultCode readProtobuf(const uint8_t *buffer, size_t msg_length);

protected:
  RadioSettings_ModeListPb m_rawSettings;
  ModeVector m_modes;
};