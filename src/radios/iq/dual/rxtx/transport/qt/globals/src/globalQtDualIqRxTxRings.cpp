#include "transport/qt/globalQtDualIqRxTxRings.h"

QtDualIqRxTxMessageRings&
globalQtDualIqRxTxRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtDualIqRxTxMessageRings rings;
  return rings;
}
