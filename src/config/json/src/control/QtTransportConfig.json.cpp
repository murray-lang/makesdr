#include "config/json/QtTransportConfig.json.h"

namespace Config::QtTransport
{
  ResultCode fromJson(const JsonVariantConst& json, Fields& fields)
  {
    if (json["role"].is<JsonVariantConst>()) {
      TargetString role = json["role"].as<const char*>();
      if (role == "radio") {
        fields.target = Radio;
      } else if (role == "client") {
        fields.target = Client;
      } else {
        return ResultCode::ERR_CONFIG_INVALID_TRANSPORT_ROLE;
      }
    } else {
      fields.target = Radio;
    }
    if (json["target"].is<JsonVariantConst>()) {
      TargetString target = json["target"].as<const char*>();
      if (target == "settings") {
        fields.message = Settings;
      } else if (target == "update") {
        fields.message = Update;
      } else if (target == "modes") {
        fields.message = Modes;
      } else if (target == "bands") {
        fields.message = Bands;
      } else if (target == "meter") {
        fields.message = Meter;
      } else if (target == "iq") {
        fields.message = Iq;
      } else {
        return ResultCode::ERR_CONFIG_INVALID_QT_TARGET;
      }
    } else {
      fields.message = Settings;
    }
    return ResultCode::OK;
  }
}