#pragma once
#include <ResultCode.h>
#include <gpio/base/GpioLineValue.h>
#include <gpio/base/GpioLineMask.h>


#ifdef USE_ETL
#include <etl/delegate.h>

using GpioLineReader = etl::delegate<ResultCode(GpioLineMask, GpioLineValue*)>;
#else
#include <functional>
using GpioLineReader = std::function<ResultCode(GpioLineMask, GpioLineValue*)>;
#endif

ResultCode gpioReadLine(GpioLineMask mask, GpioLineValue* result);
extern bool isGpioInputLineAvailable(GpioLineMask mask);


