#pragma once

#include <settings/control/radio/RadioControlT.h>
#include <radios/settings/model/SplitBandDualIqRxTxSettings.h>

#define USE_DOTTED_STRING_PATHS // Enable dotted string path resolution

using RadioControl = RadioControlT<SplitBandDualIqRxTxSettings>;
