#pragma once

#include <transport/SettingsMessageRingsT.h>
#include <transport/qt/QtNotifier.h>

template <typename RadioSettingsT>
using QtSettingsMessageRingsT = SettingsMessageRingsT<RadioSettingsT, QtNotifier>;
