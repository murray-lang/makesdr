#include "transport/qt/globalQtBasicIqRxTxRings.h"

QtBasicIqRxTxMessageRings&
globalQtIqBasicRxTxRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtBasicIqRxTxMessageRings rings;
  return rings;
}
