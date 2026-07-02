#pragma once
#include <ResultCode.h>
#include <gpio/service/Gpio.h>
#include <gpio/service/GpioInputLinesSource.h>
#include <settings/model/core/RadioSettings.h>


extern Gpio& gpio;

extern ResultCode gpioTest();

extern RadioSettings radioSettings;