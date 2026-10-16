#include "transport/qt/globalQtDualIqSplitRings.h"

QtSplitBandDualIqMessageRings&
globalQtDualIqSplitRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtSplitBandDualIqMessageRings rings;
  return rings;
}
