#pragma once

#include <ResultCode.h>
#include <config/struct/ControlConfig.h>
#include "SettingsControlSourceVariant.h"

class SettingsControlSourceFactory
{
public:
  static ResultCode create(const Config::Control::SourceConfigVariant& config, SettingsControlSourceVariant& source);
};