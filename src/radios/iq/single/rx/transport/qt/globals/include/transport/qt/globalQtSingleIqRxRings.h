#pragma once

#include <transport/qt/QtSettingsMessageRingsT.h>
#include <settings/model/radio/iq/SingleIqRxSettings.h>

using QtSingleIqRxSettingsRings = QtSettingsMessageRingsT<SingleIqRxSettings>;
// The one set of channels shared by the radio and the client in this process.
// Both sides reach it here, so neither needs to know about the other.
extern QtSingleIqRxSettingsRings& globalQtSingleIqRxRings();
