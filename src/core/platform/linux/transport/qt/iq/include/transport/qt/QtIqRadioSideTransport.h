#pragma once

#include <transport/IqRadioSideRingsT.h>

#include "QtIqMessageRings.h"

// The radio's IQ side on Qt. It only sends, so needs no wake.
using QtIqRadioSideTransport = IqRadioSideRingsT<QtIqMessageRings>;
