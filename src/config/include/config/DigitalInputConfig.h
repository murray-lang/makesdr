//
// Created by murray on 15/9/25.
//

#pragma once

#include "ConfigBase.h"
#include "GpioInputLinesConfig.h"

#ifdef USE_ETL
#include <etl/string.h>
#else
#include <string>
#endif

namespace Config::DigitalInput
{
  static constexpr auto type = "digitalinput";

  struct Fields : Alternative, GpioInputLines::Fields
  {
    SettingPathString settingPath;
  };

  extern ResultCode fromJson(JsonVariantConst json, Fields& fields);
}

// using DigitalInputConfig = Config::DigitalInput::Fields;
