#pragma once

#include <transport/IqMessageRingsT.h>
#include <transport/qt/QtNotifier.h>
#include <radios/settings/model/DualIqRxSettings.h>

using QtDualIqRxMessageRings = IqMessageRingsT<DualIqRxSettings, QtNotifier>;
