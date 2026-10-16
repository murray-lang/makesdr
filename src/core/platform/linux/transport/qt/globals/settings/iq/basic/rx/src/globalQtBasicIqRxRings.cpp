#include "transport/qt/globalQtBasicIqRxRings.h"

QtBasicIqRxMessageRings&
globalQtIqBasicRxRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtBasicIqRxMessageRings rings;
  return rings;
}
