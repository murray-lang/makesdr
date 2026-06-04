#pragma once

#include "ConfigBase.json.h"
#include "config/struct/DigitalOutputsConfig.h"

namespace Config::DigitalOutputs
{
  extern ResultCode fromJson(const JsonDocument& json, Fields& fields);
}
