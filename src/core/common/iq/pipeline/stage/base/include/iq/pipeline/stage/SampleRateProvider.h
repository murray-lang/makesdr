#pragma once

#ifdef USE_ETL
#include <etl/inplace_function.h>
using SampleRateProvider = etl::inplace_function<uint32_t()>;
#else
#include <functional>
using SampleRateProvider = std::function<uint32_t()>;
#endif