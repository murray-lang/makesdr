#include "config/json/FieldDescriptorConfig.json.h"

#include <ResultCode.h>

namespace Config::FieldDescriptor
{
  ResultCode fromJson(const JsonVariantConst& json, Fields& fields)
  {
    const auto arr = json["tags"].as<JsonArrayConst>();
    FieldPathTags tags;
    for (JsonVariantConst v : arr) {
      if (v.is<uint32_t>()) {
        tags.push_back(v.as<uint32_t>());
      } else {
        return ResultCode::ERR_CONFIG_INVALID_SETTING_PATH_TAG;
      }
    }
    fields.tags = tags;
    fields.isIndirect = json["isIndirect"].is<JsonVariantConst>() ? json["isIndirect"].as<bool>() : false;
    fields.needsAutoComplete = json["needsAutoComplete"].is<JsonVariantConst>() ? json["needsAutoComplete"].as<bool>() : false;

    return ResultCode::OK;
  }
}
