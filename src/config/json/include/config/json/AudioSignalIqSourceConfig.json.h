#pragma once

#include "ConfigBase.json.h"
#include "config/struct/AudioSignalIqSourceConfig.h"

namespace Config::AudioSignalIqSource
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}

// using AudioSignalIqSourceConfig = Config::AudioSignalIqSource::Fields;
