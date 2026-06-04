#pragma once
#include "settings/model/core/RadioSettings.h"
#include "settings/model/core/RadioSettingsSink.h"

class RadioSettingsSource
{
public:
  virtual ~RadioSettingsSource() = default;
  virtual void connectSink(RadioSettingsSink& sink) = 0;

protected:
  virtual ResultCode notifySettings(const RadioSettings& settings) = 0;
};
