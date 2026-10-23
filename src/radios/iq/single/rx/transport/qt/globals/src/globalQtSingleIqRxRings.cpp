#include "transport/qt/globalQtSingleIqRxRings.h"

QtSingleIqRxSettingsRings&
globalQtSingleIqRxRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtSingleIqRxSettingsRings rings;
  return rings;
}
