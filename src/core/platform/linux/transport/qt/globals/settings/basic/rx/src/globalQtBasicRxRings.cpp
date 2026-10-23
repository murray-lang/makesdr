#include "transport/qt/globalQtBasicRxRings.h"

QtBasicRxMessageRings&
globalQtBasicRxRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtBasicRxMessageRings rings;
  return rings;
}
