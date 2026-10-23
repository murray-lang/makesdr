#include "transport/qt/globalQtSingleIqRxTxRings.h"

QtSingleIqRxTxMessageRings&
globalQtIqBasicRxTxRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtSingleIqRxTxMessageRings rings;
  return rings;
}
