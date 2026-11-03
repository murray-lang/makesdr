#pragma once

#include <transport/MetaMessageRingsT.h>
#include <transport/RxMeteringMessageRingsT.h>
#include <transport/SettingsMessageRingsT.h>
#include <transport/SpectrumMessageRingsT.h>
#include <transport/UpdateMessageRingsT.h>

#include "HsemNotifier.h"

// The ring families shared between the STM32H745's CM7 and CM4: the
// counterparts of QtMetaMessageRings, QtSettingsMessageRingsT<> and so on.
using Stm32H745MetaMessageRings       = MetaMessageRingsT<HsemNotifier>;
using Stm32H745RxMeteringMessageRings = RxMeteringMessageRingsT<HsemNotifier>;
using Stm32H745UpdateMessageRings     = UpdateMessageRingsT<HsemNotifier>;
using Stm32H745SpectrumMessageRings   = SpectrumMessageRingsT<HsemNotifier>;

template<typename RadioSettingsT>
using Stm32H745SettingsMessageRingsT = SettingsMessageRingsT<RadioSettingsT, HsemNotifier>;
