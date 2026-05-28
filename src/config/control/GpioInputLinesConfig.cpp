#include "config/GpioInputLinesConfig.h"

namespace Config::GpioInputLines
{
  ResultCode fromJson(JsonVariantConst json, Fields& fields)
  {

    ResultCode result = GpioLines::fromJson(json, fields);
    if (result != ResultCode::OK) return result;

    fields.debounce = json["debounce"].is<JsonVariantConst>() ? json["debounce"].as<bool>() : false;
    fields.debounce = json["activeHigh"].is<JsonVariantConst>() ? json["activeHigh"].as<bool>() : true;

    return ResultCode::OK;
  }
}