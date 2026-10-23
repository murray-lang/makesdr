#include "transport/qt/globalQtIqRings.h"

QtIqMessageRings&
globalQtIqRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtIqMessageRings rings;
  return rings;
}
