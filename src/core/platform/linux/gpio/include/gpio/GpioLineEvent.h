#pragma once
#include "GpioLineMask.h"
#include "GpioTimestamp.h"

struct GpioLineEvent
{
  GpioLineMask mask;
  bool changed;
  int32_t value;
  GpioTimestamp timestamp;
};


