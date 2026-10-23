#include "transport/qt/globalQtDualIqRxRings.h"

QtDualIqRxSettingsRings&
globalQtDualIqRxRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtDualIqRxSettingsRings rings;
  return rings;
}
