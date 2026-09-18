#include "config/json/DigitalOutputConfig.json.h"
#include "config/json/GpioLinesConfig.json.h"
#include "config/json/FieldDescriptorConfig.json.h"


namespace Config::DigitalOutput
{
  ResultCode fromJson(const JsonVariantConst& json, Fields& fields)
  {
    fields.type = type;
    ResultCode result =
      GpioLines::fromJson(json, static_cast<Config::GpioLines::Fields&>(fields));
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