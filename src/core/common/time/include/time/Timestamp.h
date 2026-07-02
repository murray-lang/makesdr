#pragma once
#include <cstdint>

#if TIMESTAMP_WIDTH == 32
using Timestamp = uint32_t;
#elif TIMESTAMP_WIDTH == 64
using Timestamp = uint64_t;
#else
using Timestamp = uint32_t;
#endif

extern Timestamp now();