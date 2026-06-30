#include <stm32h745i/drivers/hal/stm32h7xx_hal.h>
#include <stdint.h>

#include "stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h"

void _HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  // TODO: Get this out to the application.
  // if (GPIO_Pin == GPIO_PIN_13)
  // {
  //   /* Toggle LED2 */
  //   BSP_LED_Toggle(LED2);
  // }
}
