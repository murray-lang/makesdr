#pragma once

#include "ConfigBase.json.h"
#include "config/struct/QtControlSourceConfig.h"


namespace Config::QtControlSource
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}