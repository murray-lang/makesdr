#pragma once

#include <radios/iq/QtSplitBandDualIqMessageRings.h>

// The one set of channels shared by the radio and the client in this process.
// Both sides reach it here, so neither needs to know about the other.
extern QtSplitBandDualIqMessageRings& globalQtDualIqSplitRings();
