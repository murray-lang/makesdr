#pragma once

#include "ConfigBase.json.h"
#include "config/struct/QtTransportOutConfig.h"

namespace Config::QtTransportOut
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}