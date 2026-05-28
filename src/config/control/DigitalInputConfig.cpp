#include "config/DigitalInputConfig.h"

namespace Config::DigitalInput
{
  ResultCode fromJson(JsonVariantConst json, Fields& fields)
  {
    fields.type = type;
    ResultCode result = GpioInputLines::fromJson(json, fields);
    if (result != ResultCode::OK) return result;

    if (json["settingPath"].is<JsonVariantConst>()) {
      fields.settingPath = json["settingPath"].as<const char *>();
    } else {
      return ResultCode::ERR_CONFIG_MISSING_SETTING_PATH;
    }

    return ResultCode::OK;
  }
}