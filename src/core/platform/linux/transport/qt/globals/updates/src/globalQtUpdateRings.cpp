#include "transport/qt/globalQtUpdateRings.h"

QtUpdateMessageRings&
globalQtUpdateRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtUpdateMessageRings rings;
  return rings;
}
