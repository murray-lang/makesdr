#ifndef STM32_GPIOINPUTS_H
#define STM32_GPIOINPUTS_H

#include "../config.h"
#include <ResultCode.h>
#include <gpio/base/GpioLineMask.h>
#include <gpio/base/GpioLineValue.h>

extern ResultCode gpioReadLine(GpioLineMask mask, GpioLineValue* result);
extern bool isGpioInputLineAvailable(GpioLineMask mask);

#endif // STM32_GPIOINPUTS_H