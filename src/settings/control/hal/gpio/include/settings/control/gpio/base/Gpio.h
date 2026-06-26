#pragma once

#include <gpiod.h>

#include "GpioInputLinesRequest_old.h"
#include "GpioOutputLinesRequest.h"
#include "GpioLinesConfig.h"
#include "GpioLinesRequestBase.h"
/*
#ifdef USE_GPIOD
#include "impl/gpiod/GpioGpiod.h"
#elif defined( USE_PIGPIO)
#include "impl/pigpio/GpioPiGpio.h"
#endif
*/

#ifdef USE_ETL
using GpioConsumerString = etl::string<MAX_GPIO_CONTEXT_LENGTH>;
#else
using GpioConsumerString = std::string;
#endif

class Gpio
{
public:
  Gpio();
  virtual ~Gpio();

  static Gpio& getInstance() {
    static Gpio instance; // Only created once, thread-safe since C++11
    return instance;
  }

  // Delete copy and move operations
  Gpio(const Gpio&) = delete;
  Gpio& operator=(const Gpio&) = delete;
  Gpio(Gpio&&) = delete;
  Gpio& operator=(Gpio&&) = delete;

  static bool isPresent();
  ResultCode open();
  void close();

  ResultCode requestInputs(
    const char * contextId,
    const GpioInputLinesVector& inputs,
    GpioInputLinesRequest_old& lr
  );

  ResultCode requestOutput(
    const char * contextId,
    const GpioLinesConfig& output,
    GpioOutputLinesRequest& lr
  );

protected:
  void* m_pChip;
};