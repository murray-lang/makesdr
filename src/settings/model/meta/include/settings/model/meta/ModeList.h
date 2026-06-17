#pragma once
#include <CrossPlatformTypes.h>

#include <ResultCode.h>
#include "settings/model/proto/RadioSettings.pb.h"


#define MAX_MODE_COUNT 10

#ifdef USE_ETL
#include <etl/span.h>


using ModeSpan = etl::span<const makesdr_ModePb, MAX_MODE_COUNT>;
#else
using ModeSpan = std::span<makesdr_ModePb>;
#endif

class ModeList
{
public:
  ModeList(const makesdr_ModeListPb& raw);

  // Allow moving if needed
  ModeList(ModeList&&)  noexcept = default;
  // ModeList& operator=(ModeList&&)  noexcept = default;

  [[nodiscard]] const ModeSpan& modes() const { return m_modes; }

  [[nodiscard]] const makesdr_ModePb* findModeByType(makesdr_ModeType type) const;

protected:
  const makesdr_ModeListPb& m_rawSettings;
  ModeSpan m_modes;
};