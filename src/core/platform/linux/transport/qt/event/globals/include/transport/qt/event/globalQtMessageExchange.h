#pragma once

#include <settings/model/radios/RadioSettings.h>
#include <transport/radio/RadioMessageExchangeT.h>
#include <transport/qt/event/QtNotifier.h>

using QtRadioMessageExchange = RadioMessageExchangeT<RadioSettings, QtNotifier>;

// The one set of channels shared by the radio and the client in this process.
// Both sides reach it here, so neither needs to know about the other.
QtRadioMessageExchange& globalQtMessageExchange();
