#pragma once
#include "ResultCode.h"
#include "base/SettingFieldUpdate.h"

class ResolveIndirection{
public:
  virtual ~ResolveIndirection() = default;
  virtual ResultCode resolveIndirection(
    const SettingFieldPath& indirectPath,
    uint32_t startingAtIndex,
    SettingFieldPath& resolvedPath
    ) = 0;
};
