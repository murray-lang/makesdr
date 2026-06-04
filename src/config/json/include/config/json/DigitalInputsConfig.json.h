#pragma once

#include "TypedJson.h"
#include "config/struct/DigitalInputsConfig.h"

namespace Config::DigitalInputs
{
  extern ResultCode fromJson(const TypedJson& json, Fields& fields);
}

