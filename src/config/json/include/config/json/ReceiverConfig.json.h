#pragma once

#include "ConfigBase.json.h"
#include "config/struct/ReceiverConfig.h"


namespace Config::Receiver
{
  extern ResultCode fromJson(JsonVariantConst json, Fields& fields);
}
