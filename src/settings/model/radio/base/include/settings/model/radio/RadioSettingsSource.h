#pragma once

#include "IRadioSettings.h"
#include "RadioSettingsSink.h"

class RadioSettingsSource
{
public:
  virtual ~RadioSettingsSource() = default;
  virtual void connectRadioSettingsSink(RadioSettingsSink* sink) = 0;

protected:
  virtual ResultCode notifySettings(IRadioSettings& settings) = 0;
};

