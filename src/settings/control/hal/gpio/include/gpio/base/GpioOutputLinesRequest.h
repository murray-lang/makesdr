#pragma once
#include "GpioLinesRequestBase.h"
#include "ResultCode.h"

class GpioOutputLinesRequest : public GpioLinesRequestBase
{
public:
  GpioOutputLinesRequest() = default;
  ~GpioOutputLinesRequest() override = default;

  virtual ResultCode request(const char* contextId, const GpioLines& input) = 0;
};
