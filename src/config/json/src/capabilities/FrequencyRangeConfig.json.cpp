#include "config/json/FrequencyRangeConfig.json.h"

namespace Config::FrequencyRange
{
  ResultCode fromJson(const JsonVariantConst& json, Fields& fields)
  {
    if (json["min"].is<JsonVariantConst>()) {
      fields.min = json["min"].as<int64_t>();
    } else {
      fields.min = 0;
    }
    if (json["max"].is<JsonVariantConst>()) {
      fields.max = json["max"].as<int64_t>();
    } else {
      fields.max = 0;
    }
    return ResultCode::OK;
  }
}
