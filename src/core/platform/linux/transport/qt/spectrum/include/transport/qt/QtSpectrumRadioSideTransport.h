#pragma once

#include <transport/SpectrumRadioSideRingsT.h>

#include "QtSpectrumMessageRings.h"

// The radio's IQ side on Qt. It only sends, so needs no wake.
using QtSpectrumRadioSideTransport = SpectrumRadioSideRingsT<QtSpectrumMessageRings>;
