#pragma once

#include <stm32h745i/drivers/conf/hw_semaphore.h>
#include <stdio.h>
#include <stdint.h>

#define SAFE_PRINTF(fmt, ...) \
  do { \
    LOCK_HSEM(UART_HSEM_ID); \
    printf(fmt, ##__VA_ARGS__); \
    UNLOCK_HSEM(UART_HSEM_ID); \
  } while(0)
