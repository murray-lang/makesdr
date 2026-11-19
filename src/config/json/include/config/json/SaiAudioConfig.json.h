#pragma once
#include "ConfigBase.json.h"
#include "config/struct/SaiAudioConfig.h"

namespace Config::SaiAudio
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}
