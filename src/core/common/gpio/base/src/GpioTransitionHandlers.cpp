#include "gpio/input/GpioInputLinesRequest.h"
#include "gpio/input/handlers/GpioTransitionHandlers.h"


#ifdef USE_ETL
#include <etl/variant.h>
#else
#include <variant>
#endif

GpioLineTransitionHandlers::GpioLineTransitionHandlers()
  : m_handlers{}
{
  for (int i = 0; i < MAX_GPIO_LINES; i++) m_handlers[i] = nullptr;
}

GpioLineTransitionHandler*
GpioLineTransitionHandlers::getTransitionHandler(GpioLineMask lineMask)
{
  int lineNo = __builtin_ctz(lineMask);
  return m_handlers[lineNo];
}

GpioLineTransitionHandler*
GpioLineTransitionHandlers::addTransitionHandler(GpioLineMask lineMask, GpioInputLinesRequest* request)
{
  // BSP_LED_On(LED_GREEN);

  if (request->config.isDebounce()) {
    int lineNo = __builtin_ctz(lineMask);
    m_handlerStorage.emplace_back<GpioLineDebouncer>({
      lineMask,
      request->config.isActiveHigh(),
      request->lineReader,
      10
    });
#ifdef USE_ETL
    m_handlers[lineNo] = etl::get_if<GpioLineDebouncer>(&m_handlerStorage.back());
#else
    m_handlers[lineNo] = std::get_if<GpioLineDebouncer>(&m_handlerStorage.back());
#endif
    return m_handlers[lineNo];
  }
  if (request->config.isRotaryEncoder()) {

    GpioLineMask lineA = 0;
    GpioLineMask lineB = 0;
    for (int i = 0; i < MAX_GPIO_LINES; i++) {
      GpioLineMask nextLine = lineMask & (1 << i);
      if (nextLine == 0) continue;
      if (lineA == 0) {
        lineA = nextLine;
      } else if (lineB == 0) {
        lineB = nextLine;
        break;
      }
    }
    if (lineA == 0 || lineB == 0) return nullptr;
    m_handlerStorage.emplace_back<GpioRotaryEncoder>({lineA, lineB, request->lineReader});

    int lineNoA = __builtin_ctz(lineA);
    int lineNoB = __builtin_ctz(lineB);
#ifdef USE_ETL
    m_handlers[lineNoA] = etl::get_if<GpioRotaryEncoder>(&m_handlerStorage.back());
#else
    m_handlers[lineNoA] = std::get_if<GpioRotaryEncoder>(&m_handlerStorage.back());
#endif
    m_handlers[lineNoB] = m_handlers[lineNoA];
    return m_handlers[lineNoA];
  }
  return nullptr;
}
