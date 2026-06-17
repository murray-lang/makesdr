#pragma once

#include "ResultCode.h"
#include "settings/model/core/SettingUpdate.h"

class SettingUpdateSink
{
public:
  virtual ~SettingUpdateSink() = default;

  virtual ResultCode applySettingUpdate(const SettingUpdate& update) = 0;
};