#pragma once

#include "ConfigBase.json.h"
#include "config/struct/RadioConfig.h"

namespace Config::Radio
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}

// extern Config::Radio::Fields tempConfig;

// using RadioConfig = Config::Radio::Fields;