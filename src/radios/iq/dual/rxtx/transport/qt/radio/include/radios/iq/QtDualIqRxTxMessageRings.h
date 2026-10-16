#pragma once

#include <transport/IqMessageRingsT.h>
#include <transport/qt/QtNotifier.h>
#include <radios/settings/model/DualIqRxTxSettings.h>

using QtDualIqRxTxMessageRings = IqMessageRingsT<DualIqRxTxSettings, QtNotifier>;
