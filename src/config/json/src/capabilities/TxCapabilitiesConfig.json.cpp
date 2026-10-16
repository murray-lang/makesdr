#include "config/json/TxCapabilitiesConfig.json.h"

namespace Config::TxCapabilities
{
  ResultCode fromJson(const JsonVariantConst& json, Fields& fields)
  {
    if (json["splitBand"].is<JsonVariantConst>()) {
      fields.splitBand = json["splitBand"].as<bool>();
    } else {
      fields.splitBand = false;
    }
    return ResultCode::OK;
  }
}
