#pragma once

#include "ConfigBase.json.h"
#include "config/struct/RotaryEncoderConfig.h"

namespace Config::RotaryEncoder
{
  extern ResultCode fromJson(JsonVariantConst json, Fields& fields);
}

// using RotaryEncoderConfig = Config::RotaryEncoder::Fields;

