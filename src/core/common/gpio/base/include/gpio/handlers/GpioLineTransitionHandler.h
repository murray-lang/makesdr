#pragma once
#include "gpio/base/GpioLineMask.h"
#include "gpio/base/GpioTimestamp.h"

#ifdef USE_ETL
#include <etl/vector.h>
#else
#include <vector>
#endif

class GpioLineTransitionHandler
{
public:
  virtual ~GpioLineTransitionHandler() = default;

  virtual bool handleLineTransition(GpioLineMask mask, GpioTimestamp timestamp, GpioLineEvent* pEvent) = 0;
};

#ifdef USE_ETL
using GpioLineTransitionHandlerVector = etl::vector<GpioLineTransitionHandler*, MAX_GPIO_LINES>;
#else
using GpioLineTransitionHandlerVector = std::vector<GpioLineTransitionHandler*>;
#endif
