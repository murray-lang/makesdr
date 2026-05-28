#pragma once
#include "MutableRadioSettings.h"

class MutableRadioSettingsSink{
public:
  virtual ~MutableRadioSettingsSink() = default;
  virtual ResultCode ApplySettings(MutableRadioSettings& settings) = 0;
};
