#pragma once

#include "GpioLineEvent.h"

#ifdef IS_STM32
#include <etl/delegate.h>

using GpioLineEventCallback = etl::delegate<void(GpioLineEvent*)>;
#elif defined(IS_LINUX)
#include <functional>

using GpioLineEventCallback = std::function<void(GpioLineEvent*)>;
#endif