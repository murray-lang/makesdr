#pragma once
#include "gpio/input/handlers/GpioLineTransitionHandlerVariant.h"
#include "gpio/input/GpioInputLinesRequest.h"

#ifdef USE_ETL
#include <etl/vector.h>
#else
#include <vector>
#endif

#ifdef USE_ETL
using GpioLineTransitionHandlerVariantVector = etl::vector<GpioLineTransitionHandlerVariant, MAX_GPIO_LINES>;
#else
using GpioLineTransitionHandlerVariantVector = std::vector<GpioLineTransitionHandlerVariant>;
#endif

class GpioLineTransitionHandlers
{
public:
  GpioLineTransitionHandlers();

  GpioLineTransitionHandler* addTransitionHandler(GpioLineMask lineMask, GpioInputLinesRequest* request);

  GpioLineTransitionHandler* getTransitionHandler(GpioLineMask lineMask);

  GpioLineTransitionHandlerVariantVector& getHandlers() { return m_handlerStorage; }

protected:
  // void continueDebouncing();

  GpioLineTransitionHandler* m_handlers[MAX_GPIO_LINES];
  GpioLineTransitionHandlerVariantVector m_handlerStorage;
};