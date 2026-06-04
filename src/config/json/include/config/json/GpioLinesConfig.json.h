//
// Created by murray on 29/9/25.
//

#pragma once

#include "ConfigBase.json.h"
#include "config/struct/GpioLinesConfig.h"

namespace Config::GpioLines
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
};

// using GpioLinesConfig = Config::GpioLines::Fields;

