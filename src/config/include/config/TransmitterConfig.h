#pragma once

#include "ConfigBase.h"
#include "IqIoConfig.h"

namespace Config::Transmitter
{
  static constexpr auto type = "transmitter";

  struct Fields
  {
    IqIo::Fields iqIo;
  };

  extern ResultCode fromJson(JsonVariantConst json, Fields& fields);
}

// using TransmitterConfig = Config::Transmitter::Fields;