#pragma once

#include "ConfigBase.json.h"
#include "config/struct/IqIoConfig.h"


namespace Config::IqIo
{
  extern ResultCode fromJson(JsonVariantConst json, Fields& fields);
}

// using IqIoConfig = Config::IqIo::Fields;
