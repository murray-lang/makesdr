#pragma once

#ifdef USE_ETL
#include <etl/inplace_function.h>
using AgcGainDbProvider = etl::inplace_function<float()>;
#else
#include <functional>
using AgcGainDbProvider = std::function<float()>;
#endif
