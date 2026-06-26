#pragma once
#include "GpioTimestamp.h"

class GpioNowGetter
{
public:
  [[nodiscard]] virtual GpioTimestamp now() const = 0;
};
