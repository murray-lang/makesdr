#include "config/json/QtTransportInConfig.json.h"
#include "config/json/QtTransportConfig.json.h"

namespace Config::QtTransportIn
{
  ResultCode fromJson(const JsonVariantConst& json, Fields& fields)
  {
    fields.type = type;
    ResultCode rc = QtTransport::fromJson(json, fields);
    if (rc != ResultCode::OK) return rc;
    if (fields.target == QtTransport::Target::Radio) {
      if (fields.message != QtTransport::Message::Settings) {
        return ResultCode::ERR_CONFIG_INVALID_QT_TARGET_FOR_RADIO_INPUT;
      }
    }
    return ResultCode::OK;
  }
}