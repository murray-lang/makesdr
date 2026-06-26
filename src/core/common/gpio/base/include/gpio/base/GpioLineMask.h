#pragma once
#include <cstdint>

#if LINE_MASK_WIDTH == 32
  using GpioLineMask = uint32_t;
#elif LINE_MASK_WIDTH == 16
  using GpioLineMask = uint16_t;
#else
using GpioLineMask = uint16_t;
#endif

constexpr uint16_t MAX_GPIO_LINES = LINE_MASK_WIDTH;

