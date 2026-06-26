#pragma once
#include "GpioInputLinesConfig.h"
#include "GpioLinesRequestBase.h"
#include "ResultCode.h"

class GpioInputLinesRequest_old : public GpioLinesRequestBase
{
public:
  GpioInputLinesRequest_old() = default;
  ~GpioInputLinesRequest_old() override = default;

  virtual ResultCode request(const char* contextId, const GpioInputLinesVector& inputs) = 0;
};
