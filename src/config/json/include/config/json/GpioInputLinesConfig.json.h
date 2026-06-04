#pragma once
#include "ConfigBase.json.h"
#include "config/struct/GpioInputLinesConfig.h"

namespace Config::GpioInputLines
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
};