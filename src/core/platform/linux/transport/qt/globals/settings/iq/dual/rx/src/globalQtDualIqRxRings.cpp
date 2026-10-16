#include "transport/qt/globalQtDualIqRxRings.h"

QtDualIqRxMessageRings&
globalQtDualIqRxRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtDualIqRxMessageRings rings;
  return rings;
}
