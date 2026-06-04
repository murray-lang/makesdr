#pragma once

#include "ConfigBase.json.h"
#include "config/struct/DigitalInputConfig.h"

namespace Config::DigitalInput
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}
