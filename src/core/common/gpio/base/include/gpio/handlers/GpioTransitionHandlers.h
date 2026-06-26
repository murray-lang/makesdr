#pragma once
#include "GpioLineTransitionHandlerVariant.h"
#include "gpio/base/GpioInputLinesRequest.h"

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
  GpioLineTransitionHandlers(GpioNowGetter* nowGetter);

  GpioLineTransitionHandler* addTransitionHandler(GpioLineMask lineMask, GpioInputLinesRequest& request);

  GpioLineTransitionHandler* getTransitionHandler(GpioLineMask lineMask);

  GpioLineTransitionHandlerVariantVector& getHandlers() { return m_handlerStorage; }

protected:
  void continueDebouncing();

  GpioNowGetter* m_nowGetter;
  GpioLineTransitionHandler* m_handlers[MAX_GPIO_LINES];
  GpioLineTransitionHandlerVariantVector m_handlerStorage;
};