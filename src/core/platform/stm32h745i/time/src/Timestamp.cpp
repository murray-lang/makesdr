#include <time/Timestamp.h>
#include <stm32h745i/drivers/hal/stm32h7xx_hal.h>

Timestamp now()
{
  return HAL_GetTick();
}