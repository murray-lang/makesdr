#pragma once

#include "IRadioSettings.h"

class RadioSettingsSink
{
public:
  virtual ~RadioSettingsSink() = default;
  virtual ResultCode applySettings(IRadioSettings& settings) = 0;
};
