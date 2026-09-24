#include "transport/qt/event/globalQtMessageExchange.h"

QtRadioMessageExchange&
globalQtMessageExchange()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtRadioMessageExchange exchange;
  return exchange;
}
