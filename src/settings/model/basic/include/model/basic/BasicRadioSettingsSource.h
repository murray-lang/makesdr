#pragma once
#include "BasicRadioSettings.h"

class BasicRadioSettingsSource
{
public:
  virtual ~BasicRadioSettingsSource() = default;
protected:
  virtual ResultCode notifySettings(const BasicRadioSettings& settings) = 0;
};
