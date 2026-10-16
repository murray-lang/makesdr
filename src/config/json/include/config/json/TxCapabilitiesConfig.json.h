#pragma once
#include "ConfigBase.json.h"
#include "config/struct/TxCapabilitiesConfig.h"

namespace Config::TxCapabilities
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}
