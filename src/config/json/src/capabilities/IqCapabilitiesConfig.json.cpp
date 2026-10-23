#include "config/json/IqCapabilitiesConfig.json.h"

namespace Config::IqCapabilities
{
  ResultCode fromJson(const JsonVariantConst& json, Fields& fields)
  {
    if (json["dualPipeline"].is<JsonVariantConst>()) {
      fields.dualPipeline = json["dualPipeline"].as<bool>();
    } else {
      fields.dualPipeline = false;
    }
    return ResultCode::OK;
  }
}
