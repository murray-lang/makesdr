#pragma once

#include <transport/BasicMessageRingsT.h>
#include <transport/qt/QtNotifier.h>
#include <settings/model/radio/basic/BasicRxSettings.h>

using QtBasicRxMessageRings = BasicMessageRingsT<BasicRxSettings, QtNotifier>;

// The one set of channels shared by the radio and the client in this process.
// Both sides reach it here, so neither needs to know about the other.
extern QtBasicRxMessageRings& globalQtBasicRxRings();
