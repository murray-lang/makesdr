#pragma once

#include "GpioInputLines.h"
#include "GpioLineEventCallback.h"
#include "GpioLineReader.h"
#include "GpioCheckedLineReader.h"
#include "gpio/base/GpioInputLines.h"

#ifdef USE_ETL
#include <etl/vector.h>
#else
#include <vector>
#endif


class GpioInputLinesRequest : public GpioInputLines
{
public:
  GpioInputLinesRequest(GpioInputLines& config, GpioLineEventCallback& callback)
    : config(config)
    , lineEventCallback(callback)
  {
  }

  GpioInputLines& config;
  GpioLineEventCallback& lineEventCallback;
  GpioCheckedLineReader lineReader;
};

#ifdef USE_ETL
using GpioInputLinesRequestVector = etl::vector<GpioInputLinesRequest, MAX_GPIO_LINES_PER_DEVICE>;
#else
using GpioInputLinesRequestVector = std::vector<GpioInputLinesRequest>;
#endif