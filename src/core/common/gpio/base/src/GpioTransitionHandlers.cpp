#include "gpio/handlers/GpioTransitionHandlers.h"

#ifdef USE_ETL
#include <etl/variant.h>
#else
#include <variant>
#endif

GpioLineTransitionHandlers::GpioLineTransitionHandlers(GpioNowGetter* nowGetter)
  : m_nowGetter(nowGetter), m_handlers{}
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
GpioLineTransitionHandlers::addTransitionHandler(GpioLineMask lineMask, GpioInputLinesRequest& request)
{
  int lineNo = __builtin_ctz(lineMask);

  if (request.config.isDebounce()) {
    m_handlerStorage.emplace_back<GpioLineDebouncer>({
      lineMask,
      request.config.isActiveHigh(),
      request.lineReader.getLineReaderDelegate(),
      m_nowGetter,
      10
    });
#ifdef USE_ETL
    m_handlers[lineNo] = etl::get_if<GpioLineDebouncer>(&m_handlerStorage.back());
#else
    m_handlers[lineNo] = std::get_if<GpioLineDebouncer>(&m_handlerStorage.back());
#endif
    return m_handlers[lineNo];
  }
  if (request.config.isRotaryEncoder()) {
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
    m_handlerStorage.emplace_back<GpioRotaryEncoder>({lineA, lineB, request.lineReader.getLineReaderDelegate()});
#ifdef USE_ETL
    m_handlers[lineNo] = etl::get_if<GpioRotaryEncoder>(&m_handlerStorage.back());
#else
    m_handlers[lineNo] = std::get_if<GpioRotaryEncoder>(&m_handlerStorage.back());
#endif
    return m_handlers[lineNo];
  }
  return nullptr;
}
