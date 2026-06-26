#pragma once
#include <cstdint>

#if TIMESTAMP_WIDTH == 32
using GpioTimestamp = uint32_t;
#elif TIMESTAMP_WIDTH == 64
using GpioTimestamp = uint64_t;
#else
using GpioTimestamp = uint32_t;
#endif