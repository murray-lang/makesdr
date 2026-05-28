#pragma once
#include "ResultCode.h"
#include "BasicRadioSettings.h"

class BasicRadioSettingsSink
{
public:
  virtual ~BasicRadioSettingsSink() = default;
  virtual ResultCode applySettings(const BasicRadioSettings& settings) = 0;
};
