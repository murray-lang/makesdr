#pragma once

#include "QtDualIqRxTxMessageRings.h"
#include <transport/IqRadioSideRingsT.h>
#include <transport/qt/QtIqRadioSideTransport.h>

using Rings = IqRadioSideRingsT<QtDualIqRxTxMessageRings>;

using DualIqRxTxTransport = QtIqRadioSideTransport<Rings>;