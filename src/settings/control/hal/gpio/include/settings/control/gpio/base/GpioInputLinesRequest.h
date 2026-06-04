#pragma once
#include "GpioInputLines.h"
#include "GpioLinesRequestBase.h"
#include "ResultCode.h"

class GpioInputLinesRequest : public GpioLinesRequestBase
{
public:
  GpioInputLinesRequest() = default;
  ~GpioInputLinesRequest() override = default;

  virtual ResultCode request(const char* contextId, const GpioInputLinesVector& inputs) = 0;
};
