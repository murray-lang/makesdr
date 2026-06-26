#pragma once
#include "ResultCode.h"
#include "GpioLinesConfig.h"

class GpioLinesRequestBase
{
public:
  virtual ~GpioLinesRequestBase() = default;

  virtual void initialise(void* gpioHandle, const char * context) = 0;

  // virtual ResultCode request(const char * contextId, const GpioLinesVector& inputs) = 0;
  // virtual ResultCode request(const char * contextId, const GpioLines& inputs) = 0;

  virtual void release() = 0;
};
