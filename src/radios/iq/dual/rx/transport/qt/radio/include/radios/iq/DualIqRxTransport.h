#pragma once

#include "QtDualIqRxMessageRings.h"
#include <transport/IqRadioSideRingsT.h>
#include <transport/qt/QtIqRadioSideTransport.h>

using Rings = IqRadioSideRingsT<QtDualIqRxMessageRings>;

using DualIqRxTransport = QtIqRadioSideTransport<Rings>;