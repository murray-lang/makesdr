#pragma once

#include "ConfigBase.json.h"
#include "config/struct/AudioIqSourceConfig.h"

namespace Config::AudioIqSource
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}

// using AudioIqSourceConfig = Config::AudioIqSource::Fields;
