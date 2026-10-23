#pragma once

#include <transport/RxMeteringRadioSideRingsT.h>

#include "QtRxMeteringMessageRings.h"

// The radio's meta side on Qt. It only sends, so needs no wake.
using QtRxMeteringRadioSideTransport = RxMeteringRadioSideRingsT<QtRxMeteringMessageRings>;
