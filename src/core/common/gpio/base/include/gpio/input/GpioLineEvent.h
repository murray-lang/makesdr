#pragma once
#include <gpio/base/GpioLineMask.h>
#include <gpio/base/GpioLineValue.h>
#include <time/Timestamp.h>

struct GpioLineEvent
{
  GpioLineMask mask;
  bool changed;
  GpioLineValue value;
  Timestamp timestamp;
};


