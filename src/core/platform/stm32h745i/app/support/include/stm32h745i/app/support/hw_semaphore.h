#pragma once

#include <stm32h745i/drivers/hal/stm32h7xx_hal.h>

#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#define UART_HSEM_ID (2U)

#define LOCK_HSEM(__sem__) while(HAL_HSEM_FastTake(__sem__) != HAL_OK) {}
#define UNLOCK_HSEM(__sem__)  HAL_HSEM_Release(__sem__, 0)