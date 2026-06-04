#pragma once

#include "ConfigBase.json.h"
#include "config/struct/UiConfig.h"

namespace Config::Ui
{
  extern ResultCode fromJson(JsonVariantConst json, Fields& fields);
}
