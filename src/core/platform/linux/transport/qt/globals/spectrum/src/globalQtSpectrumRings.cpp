#include "transport/qt/globalQtSpectrumRings.h"

QtSpectrumMessageRings&
globalQtSpectrumRings()
{
  // Constructed on first use, so it is ready whichever side reaches it first.
  static QtSpectrumMessageRings rings;
  return rings;
}
