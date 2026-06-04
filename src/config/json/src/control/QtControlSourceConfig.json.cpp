#include "config/json/QtControlSourceConfig.json.h"

namespace Config::QtControlSource
{
  ResultCode fromJson(const JsonVariantConst& json, Fields& fields)
  {
    fields.type = type;
    return ResultCode::OK;
  }
}