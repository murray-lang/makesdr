#pragma once

#include <stm32h745i/drivers/hal/stm32h7xx_hal.h>

#define HSEM_ID_0 (0U)    // Inter-core synchronisation
#define EMMC_HSEM_ID (1U) // eMMC (mmc_diskio.c)
#define UART_HSEM_ID (2U) // SAFE_PRINTF
#define I2C4_HSEM_ID  (3U) // I2C4: WM8994 codec (CM4) + FT5336 touch (CM7)
// 4..7 free;

// MAKESDR_HSEM_RING_FIRST_ID..31 are handed out by HsemNotifier, one per
// shared message ring. Keep this above every fixed ID defined above.
#ifndef MAKESDR_HSEM_RING_FIRST_ID
#define MAKESDR_HSEM_RING_FIRST_ID 8
#endif

#define LOCK_HSEM(__sem__) while(HAL_HSEM_FastTake(__sem__) != HAL_OK) {}
#define UNLOCK_HSEM(__sem__)  HAL_HSEM_Release(__sem__, 0)