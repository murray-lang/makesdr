#include "transport/qt/globalQtMetaRings.h"

QtMetaMessageRings&
globalQtMetaRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtMetaMessageRings rings;
  return rings;
}
