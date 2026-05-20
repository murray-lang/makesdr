#pragma once
#include "RadioSettings.h"
#include "band/ActiveBandSettings.h"
#include "band/BandSettings.h"

using ReadOnlyRadioSettings = RadioSettings<ActiveBandSettings<BandSettings>>;
