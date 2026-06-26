#pragma once
#include "GpioLineValue.h"
#include "GpioLineMask.h"


class GpioLineReader
{
public:
  [[nodiscard]] virtual GpioLineValue readLine() const = 0;
  [[nodiscard]] virtual GpioLineMask getLineMask() const = 0;
};