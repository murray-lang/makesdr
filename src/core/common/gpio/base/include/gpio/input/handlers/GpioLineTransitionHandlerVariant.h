#pragma once
#include <CrossPlatformTypes.h>
#include "GpioLineDebouncer.h"
#include "GpioRotaryEncoder.h"

using GpioLineTransitionHandlerVariant = variant<monostate, GpioLineDebouncer, GpioRotaryEncoder>;
