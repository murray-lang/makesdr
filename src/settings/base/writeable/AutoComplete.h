#pragma once
#include "AutoCompleteTrigger.h"
#include "ResultCode.h"
#include "../SettingFieldPath.h"

class AutoComplete
{
public:

  virtual ~AutoComplete() = default;

  virtual ResultCode autoComplete(
    const SettingFieldPath& path,
    uint32_t startingAtIndex,
    AutoCompleteTrigger trigger) = 0;
};
