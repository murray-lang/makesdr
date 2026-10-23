#pragma once

// The one settings type used by this build. Simpler radios may choose a
// smaller type here to save space; everything else refers to RadioSettings.
#include <radios/settings/model/SplitBandDualIqRxTxSettings.h>

using RadioSettings = SplitBandDualIqRxTxSettings;
