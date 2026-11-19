#ifndef STM32_GPIOOUTPUTS_H
#define STM32_GPIOOUTPUTS_H

#include "../config.h"
#include <ResultCode.h>
#include <gpio/base/GpioLineMask.h>
#include <gpio/base/GpioLineValue.h>

extern ResultCode gpioWriteLine(GpioLineMask mask, const GpioLineValue& value);
extern bool isGpioOutputLineAvailable(GpioLineMask mask);

#endif // STM32_GPIOOUTPUTS_H