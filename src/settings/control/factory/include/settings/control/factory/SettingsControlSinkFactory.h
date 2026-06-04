#pragma once

#include <config/struct/ControlConfig.h>
#include "SettingsControlSinkVariant.h"

class SettingsControlSinkFactory
{
public:
  static ResultCode create(const Config::Control::SinkConfigVariant& config, SettingsControlSinkVariant& sink);
};