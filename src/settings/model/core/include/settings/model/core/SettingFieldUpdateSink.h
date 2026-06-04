#pragma once

#include "ResultCode.h"
#include "settings/model/core/SettingFieldUpdate.h"

class SettingFieldUpdateSink
{
public:
  virtual ~SettingFieldUpdateSink() = default;

  virtual ResultCode applySettingFieldUpdate(const SettingFieldUpdate& update) = 0;
};