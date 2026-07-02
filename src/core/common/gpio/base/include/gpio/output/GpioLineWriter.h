#pragma once
#include <ResultCode.h>
#include <gpio/base/GpioLineValue.h>
#include <gpio/base/GpioLineMask.h>


#ifdef USE_ETL
#include <etl/delegate.h>

using GpioLineWriter = etl::delegate<ResultCode(GpioLineMask, const GpioLineValue& value)>;
#else
#include <functional>
using GpioLineWriter = std::function<ResultCode(GpioLineMask, const GpioLineValue& value)>;
#endif

ResultCode gpioWriteLine(GpioLineMask mask, const GpioLineValue& value);
extern bool isGpioOutputLineAvailable(GpioLineMask mask);


