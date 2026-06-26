#include "config/json/GpioInputLinesConfig.json.h"
#include "config/json/GpioLinesConfig.json.h"

namespace Config::GpioInputLines
{
  ResultCode fromJson(const JsonVariantConst& json, Fields& fields)
  {

    ResultCode result = GpioLines::fromJson(json, fields);
    if (result != ResultCode::OK) return result;

    fields.debounce = json["debounce"].is<JsonVariantConst>() ? json["debounce"].as<bool>() : false;
    fields.activeHigh = json["activeHigh"].is<JsonVariantConst>() ? json["activeHigh"].as<bool>() : true;
    fields.rotaryEncoder = json["rotaryEncoder"].is<JsonVariantConst>() ? json["rotaryEncoder"].as<bool>() : false;

    return ResultCode::OK;
  }
}