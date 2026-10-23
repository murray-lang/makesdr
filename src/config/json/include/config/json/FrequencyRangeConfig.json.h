#pragma once
#include "ConfigBase.json.h"
#include "config/struct/FrequencyRangeConfig.h"

namespace Config::FrequencyRange
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}
