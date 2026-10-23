#include "transport/qt/globalQtDualIqRxTxRings.h"

QtDualIqRxTxSettingsRings&
globalQtDualIqRxTxRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtDualIqRxTxSettingsRings rings;
  return rings;
}
