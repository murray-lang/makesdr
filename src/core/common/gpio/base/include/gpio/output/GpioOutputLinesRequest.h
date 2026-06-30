#pragma once

#include "gpio/base/GpioLines.h"
#include "GpioLineWriter.h"

#ifdef USE_ETL
#include <etl/vector.h>
#else
#include <vector>
#endif


class GpioOutputLinesRequest
{
public:
  GpioOutputLinesRequest(GpioLines& config)
    : config(config)
    , lineWriter(defaultLineWriter)
  {
  }
  GpioOutputLinesRequest(const GpioOutputLinesRequest& rhs) = default;

  GpioLines& config;
  GpioLineWriter lineWriter;

protected:
  static ResultCode defaultLineWriter(GpioLineMask mask, const GpioLineValue& value)
  {
    return ResultCode::ERR_GPIO_OUTPUT_LINE_NOT_REQUESTED;
  }
};

#ifdef USE_ETL
using GpioOutputLinesRequestVector = etl::vector<GpioOutputLinesRequest*, MAX_GPIO_LINES_PER_DEVICE>;
#else
using GpioOutputLinesRequestVector = std::vector<GpioOutputLinesRequest*>;
#endif