#pragma once
#include "ConfigBase.json.h"
#include "config/struct/CapabilitiesConfig.h"

namespace Config::Capabilities
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}
