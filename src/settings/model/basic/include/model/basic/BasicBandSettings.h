#pragma once
#include "BandSettings.h"
#include "BasicRxPipelineSettings.h"
#include "BasicTxPipelineSettings.h"

using BasicBandSettings = BandSettings< BasicRxPipelineSettings, BasicTxPipelineSettings>;
