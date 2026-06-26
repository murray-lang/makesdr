#ifndef STM32_GPIO_H_
#define STM32_GPIO_H_

#include <gpiod.h>

#include "../GpioInputLinesRequest.h"
#include "../GpioOutputLinesRequest.h"
#include "GpioLines.h"
#include "../GpioLinesRequestBase.h"

using GpioConsumerString = etl::string<MAX_GPIO_CONTEXT_LENGTH>;

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
    const GpioDigitalInputVector& inputs,
    GpioInputLinesRequest& lr
  );

  ResultCode requestOutput(
    const char * contextId,
    const GpioLines& output,
    GpioOutputLinesRequest& lr
  );
};

#endif // STM32_GPIO_H_