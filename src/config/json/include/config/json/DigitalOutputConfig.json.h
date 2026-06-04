#pragma once

#include "ConfigBase.json.h"
#include "config/struct/DigitalOutputConfig.h"

namespace Config::DigitalOutput
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}

// using DigitalOutputConfig = Config::DigitalOutput::Fields;
