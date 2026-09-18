#include "config/json/DigitalInputConfig.json.h"
#include "config/json/GpioInputLinesConfig.json.h"
#include "config/json/FieldDescriptorConfig.json.h"

namespace Config::DigitalInput
{
  ResultCode fromJson(const JsonVariantConst& json, Fields& fields)
  {
    fields.type = type;

    ResultCode result =
      Config::GpioInputLines::fromJson(json, reinterpret_cast<Config::GpioInputLines::Fields&>(fields));
    if (result != ResultCode::OK) return result;

    if (json["settingPath"].is<JsonVariantConst>()) {
      // String path
#ifdef USE_DOTTED_STRING_PATHS
      fields.settingPath = json["settingPath"].as<const char*>();
#else
      return ResultCode::ERR_CONFIG_DOTTED_STRINGS_NOT_SUPPORTED;
#endif
    } else if (json["settingDescriptor"].is<JsonVariantConst>()) {
      return FieldDescriptor::fromJson(json["settingDescriptor"], fields.settingDescriptor.value());
    } else {
      return ResultCode::ERR_CONFIG_MISSING_SETTING_PATH;
    }
    return ResultCode::OK;
  }
}
