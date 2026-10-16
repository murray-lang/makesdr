#pragma once
#include "ConfigBase.json.h"
#include "config/struct/FrequencyRangesConfig.h"

namespace Config::FrequencyRanges
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}
