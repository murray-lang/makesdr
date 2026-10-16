#pragma once

#include <transport/IqMessageRingsT.h>
#include <transport/qt/QtNotifier.h>
#include <settings/model/radio/iq/BasicIqRxSettings.h>

using QtBasicIqRxMessageRings = IqMessageRingsT<BasicIqRxSettings, QtNotifier>;

// The one set of channels shared by the radio and the client in this process.
// Both sides reach it here, so neither needs to know about the other.
extern QtBasicIqRxMessageRings& globalQtBasicIqRxRings();
