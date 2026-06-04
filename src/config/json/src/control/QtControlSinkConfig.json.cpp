#include "config/json/QtControlSinkConfig.json.h"

namespace Config::QtControlSink
{
  ResultCode fromJson(const JsonVariantConst& json, Fields& fields)
  {
    fields.type = type;
    return ResultCode::OK;
  }
}