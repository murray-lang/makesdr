#pragma once

#include "SteppableFloatSetting.h"
#include "SteppableInt64Setting.h"

#include <etl/variant.h>

using SteppableVariant = etl::variant<SteppableFloatSetting, SteppableInt64Setting>;

