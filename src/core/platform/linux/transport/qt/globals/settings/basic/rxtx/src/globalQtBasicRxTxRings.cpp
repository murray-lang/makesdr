#include "transport/qt/globalQtBasicRxTxRings.h"

QtBasicRxTxMessageRings&
globalQtBasicRxTxRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtBasicRxTxMessageRings rings;
  return rings;
}
