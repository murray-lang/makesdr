#include "transport/qt/globalQtSingleIqRxRings.h"

QtSingleIqRxMessageRings&
globalQtIqBasicRxRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtSingleIqRxMessageRings rings;
  return rings;
}
