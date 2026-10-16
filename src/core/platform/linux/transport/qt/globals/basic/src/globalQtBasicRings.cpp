#include "transport/qt/globalQtBasicRings.h"

QtBasicMessageRings&
globalQtBasicRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtBasicMessageRings rings;
  return rings;
}
