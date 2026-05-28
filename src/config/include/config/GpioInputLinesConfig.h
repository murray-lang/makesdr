#pragma once
#include "GpioLinesConfig.h"

namespace Config::GpioInputLines
{
  struct Fields : GpioLines::Fields
  {
    bool debounce;
    bool activeHigh;
  };

  extern ResultCode fromJson(JsonVariantConst json, Fields& fields);
};