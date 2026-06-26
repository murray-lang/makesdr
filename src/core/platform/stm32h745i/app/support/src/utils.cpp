#include "stm32h745i/app/support/utils.h"
#include "stm32h745i/drivers/hal/stm32h7xx_hal.h"

GpioTimestamp now()
{
  return HAL_GetTick();
}
