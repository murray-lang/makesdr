#pragma once

#include "ConfigBase.json.h"
#include "config/struct/BandSelectorConfig.h"

namespace Config::BandSelector
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}

