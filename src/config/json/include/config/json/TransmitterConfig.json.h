#pragma once

#include "ConfigBase.json.h"
#include "config/struct/TransmitterConfig.h"


namespace Config::Transmitter
{
  extern ResultCode fromJson(JsonVariantConst json, Fields& fields);
}
