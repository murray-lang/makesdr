#pragma once
#include "GpioLinesConfig.h"

namespace Config::GpioInputLines
{
  struct Fields : GpioLines::Fields
  {
    bool debounce;
    bool activeHigh;
    bool rotaryEncoder;
  };
}