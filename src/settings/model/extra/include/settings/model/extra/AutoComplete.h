#pragma once

#include <ResultCode.h>
#include "settings/model/core/SettingPath.h"
#include "settings/model/core/AutoCompleteTrigger.h"

class AutoComplete
{
public:

  virtual ~AutoComplete() = default;

  virtual ResultCode autoComplete(
    const SettingPath& path,
    uint32_t startingAtIndex,
    AutoCompleteTrigger trigger) = 0;

  virtual ResultCode autoComplete() = 0;
};
