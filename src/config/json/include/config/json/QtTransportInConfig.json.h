#pragma once

#include "ConfigBase.json.h"
#include "config/struct/QtTransportInConfig.h"


namespace Config::QtTransportIn
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}