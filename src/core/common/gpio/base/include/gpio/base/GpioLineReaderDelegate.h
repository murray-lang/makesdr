#pragma once
#include <ResultCode.h>
#include "GpioLineValue.h"
#include "GpioLineMask.h"


#ifdef USE_ETL
#include <etl/delegate.h>

using GpioLineReaderDelegate = etl::delegate<ResultCode(GpioLineMask, GpioLineValue*)>;
#else
#include <functional>
using GpioLineReaderDelegate = std::function<ResultCode(GpioLineMask, GpioLineValue*)>;
#endif

