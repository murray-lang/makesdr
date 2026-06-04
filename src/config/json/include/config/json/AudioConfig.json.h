
#pragma once
#include "ConfigBase.json.h"
#include "config/struct/AudioConfig.h"


namespace Config::Audio
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);

  extern void toJson(const Fields& fields, JsonObject& json);
}

// using AudioConfig = Config::Audio::Fields;
