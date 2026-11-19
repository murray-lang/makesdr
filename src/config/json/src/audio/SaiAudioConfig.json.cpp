#include <CrossPlatformTypes.h>
#include <algorithm>
#include "config/json/SaiAudioConfig.json.h"

namespace Config::SaiAudio
{
  // Both volumes are optional (the struct's defaults apply) and are clamped to 0..100.
  ResultCode fromJson(const JsonVariantConst& json, Fields& fields)
  {
    fields.type = type;
    if (json["outputVolume"].is<uint32_t>()) {
      fields.outputVolume = std::min<uint32_t>(json["outputVolume"].as<uint32_t>(), 100);
    }
    if (json["inputVolume"].is<uint32_t>()) {
      fields.inputVolume = std::min<uint32_t>(json["inputVolume"].as<uint32_t>(), 100);
    }
    return ResultCode::OK;
  }
}
