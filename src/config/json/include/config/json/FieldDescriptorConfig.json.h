#pragma once
#include "ConfigBase.json.h"
#include <config/struct/FieldDescriptorConfig.h>

namespace Config::FieldDescriptor
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}