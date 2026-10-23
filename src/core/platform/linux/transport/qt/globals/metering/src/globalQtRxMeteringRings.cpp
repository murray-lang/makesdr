#include "transport/qt/globalQtRxMeteringRings.h"

QtRxMeteringMessageRings&
globalQtRxMeteringRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtRxMeteringMessageRings rings;
  return rings;
}
