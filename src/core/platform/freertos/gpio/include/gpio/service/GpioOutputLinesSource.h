#pragma once
#include "gpio/output/GpioOutputLinesRequest.h"
#include "ResultCode.h"


class GpioOutputLinesSource
{
public:
  GpioOutputLinesSource() = default;
  ~GpioOutputLinesSource() = default;

  ResultCode initialise(void* gpioHandle, const char * context);
  void release();

  ResultCode requestLines(const char* context, GpioOutputLinesRequest* request);
  ResultCode requestLines(const char* context, GpioOutputLinesRequestVector& requests);

};
