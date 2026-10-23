#include "transport/qt/globalQtDualIqSplitRings.h"

QtSplitBandDualIqSettingsRings&
globalQtDualIqSplitRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtSplitBandDualIqSettingsRings rings;
  return rings;
}
