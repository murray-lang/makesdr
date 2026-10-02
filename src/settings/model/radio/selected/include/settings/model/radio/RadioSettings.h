#pragma once

// The one settings type used by this build. Simpler radios may choose a
// smaller type here to save space; everything else refers to RadioSettings.
#include <settings/model/radio/iq/SplitBandDualIqRxTxSettings.h>

using RadioSettings = SplitBandDualIqRxTxSettings;
