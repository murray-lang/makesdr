#pragma once

#include "ConfigBase.json.h"
#include "config/struct/QtControlSinkConfig.h"

namespace Config::QtControlSink
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}