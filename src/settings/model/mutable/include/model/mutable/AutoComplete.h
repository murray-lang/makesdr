#pragma once

#include <ResultCode.h>
#include "model/core/SettingFieldPath.h"
#include "model/core/AutoCompleteTrigger.h"

class AutoComplete
{
public:

  virtual ~AutoComplete() = default;

  virtual ResultCode autoComplete(
    const SettingFieldPath& path,
    uint32_t startingAtIndex,
    AutoCompleteTrigger trigger) = 0;

  virtual ResultCode autoComplete() = 0;
};
