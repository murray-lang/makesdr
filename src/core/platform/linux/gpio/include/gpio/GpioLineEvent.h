#pragma once
#include "GpioLineMask.h"
#include "Timestamp.h"

struct GpioLineEvent
{
  GpioLineMask mask;
  bool changed;
  int32_t value;
  Timestamp timestamp;
};


