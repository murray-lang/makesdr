#pragma once
#include "ConfigBase.json.h"
#include "config/struct/IqCapabilitiesConfig.h"

namespace Config::IqCapabilities
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}
