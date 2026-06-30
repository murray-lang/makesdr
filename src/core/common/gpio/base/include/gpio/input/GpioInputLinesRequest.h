#pragma once

#include "GpioInputLines.h"
#include "GpioLineEventCallback.h"
#include "GpioLineReader.h"

#ifdef USE_ETL
#include <etl/vector.h>
#else
#include <vector>
#endif


class GpioInputLinesRequest
{
public:
  GpioInputLinesRequest(GpioInputLines& config, GpioLineEventCallback& callback)
    : config(config)
    , lineEventCallback(callback)
    , lineReader(defaultLineReader)
  {
  }
  GpioInputLinesRequest(const GpioInputLinesRequest& rhs) = default;

  GpioInputLines& config;
  GpioLineEventCallback& lineEventCallback;
  GpioLineReader lineReader;

protected:
  static ResultCode defaultLineReader(GpioLineMask mask, GpioLineValue* value)
  {
    return ResultCode::ERR_GPIO_INPUT_LINE_NOT_REQUESTED;
  }
};

#ifdef USE_ETL
using GpioInputLinesRequestVector = etl::vector<GpioInputLinesRequest*, MAX_GPIO_LINES_PER_DEVICE>;
#else
using GpioInputLinesRequestVector = std::vector<GpioInputLinesRequest*>;
#endif