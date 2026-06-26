#pragma once

#include "GpioLineDebouncer.h"
#include "GpioRotaryEncoder.h"

#ifdef USE_ETL
#include <etl/variant.h>

using GpioLineTransitionHandlerVariant = etl::variant<etl::monostate, GpioLineDebouncer, GpioRotaryEncoder>;
#else
#include <variant>
using GpioLineTransitionHandlerVariant = std::variant<std::monostate, GpioLineDebouncer, GpioRotaryEncoder>;
#endif
