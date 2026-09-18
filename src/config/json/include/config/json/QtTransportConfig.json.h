#pragma once

#include "ConfigBase.json.h"
#include "config/struct/QtTransportInConfig.h"


namespace Config::QtTransport
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}