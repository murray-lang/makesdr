#pragma once

#include <settings/model/radio/iq/SingleIqRxSettings.h>
#include <transport/stm32h745i/globalStm32H745Rings.h>

using Stm32H745SingleIqRxSettingsRings = Stm32H745SettingsMessageRingsT<SingleIqRxSettings>;

// The settings rings shared by the radio (CM4) and the UI (CM7): the
// counterpart of globalQtSingleIqRxRings().
inline Stm32H745SingleIqRxSettingsRings& globalStm32H745SingleIqRxRings()
{
  return globalStm32H745SettingsRings<SingleIqRxSettings>();
}

// CM7 only, once, before waking the CM4: creates all the shared rings, with
// settings rings for this radio.
inline void createStm32H745SingleIqRxRings()
{
  createStm32H745Rings<SingleIqRxSettings>();
}
