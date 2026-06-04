#pragma once

#include "ConfigBase.json.h"
#include "config/struct/ControlConfig.h"


namespace Config::Control
{
  // extern ResultCode SinkFactory(const TypedJson& json, OptionalSinkVariant& optionalSink);
  // extern ResultCode SourceFactory(const TypedJson& json, OptionalSourceVariant& optionalSource);
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}

// using ControlConfig = Config::Control::Fields;
