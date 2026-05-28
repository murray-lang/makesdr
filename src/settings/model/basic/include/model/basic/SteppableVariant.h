#pragma once
#include <CrossPlatformTypes.h>
#include "SteppableFloatSetting.h"
#include "SteppableInt64Setting.h"

using SteppableVariant = variant<SteppableFloatSetting, SteppableInt64Setting>;

