#pragma once

#include <settings/model/radio/iq/SplitBandDualIqRxTxSettings.h>
#include <transport/qt/QtSettingsMessageRingsT.h>

using QtSplitBandDualIqSettingsRings = QtSettingsMessageRingsT<SplitBandDualIqRxTxSettings>;
// The one set of channels shared by the radio and the client in this process.
// Both sides reach it here, so neither needs to know about the other.
extern QtSplitBandDualIqSettingsRings& globalQtDualIqSplitRings();
