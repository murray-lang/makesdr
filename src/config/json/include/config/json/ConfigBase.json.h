#pragma once
#include "ResultCode.h"
#include "config/struct/ConfigBase.h"
#include <ArduinoJson.h>

namespace Config
{
  extern ResultCode fromJson(const JsonVariantConst& json, Alternative& fields);
}
