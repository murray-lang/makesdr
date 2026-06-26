#pragma once
#include "GpioLineMask.h"
#include "GpioLineValue.h"
#include "GpioTimestamp.h"

struct GpioLineEvent
{
  GpioLineMask mask;
  bool changed;
  GpioLineValue value;
  GpioTimestamp timestamp;
};


