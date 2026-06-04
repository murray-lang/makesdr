#pragma once
#include "RadioSettings.h"

class RadioSettingsSink{
public:
  virtual ~RadioSettingsSink() = default;
  virtual ResultCode applySettings(const RadioSettings& settings) = 0;
};
