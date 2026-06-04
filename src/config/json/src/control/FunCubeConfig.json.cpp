#include "config/json/FunCubeConfig.json.h"

namespace Config::FunCube
{
  ResultCode fromJson(const JsonVariantConst& json, Fields& fields)
  {
    fields.type = type;
    return ResultCode::OK;
  }
}