/*
 * TinyUSB Wrapper for STM32H745I-DISCO
 * This header sets platform-specific defines before including tusb.h
 * Use this instead of including tusb.h directly
 */

#ifndef TUSB_WRAPPER_H_
#define TUSB_WRAPPER_H_

#ifdef __cplusplus
 extern "C" {
#endif

// Define MCU type for TinyUSB
#ifndef CFG_TUSB_MCU
#define CFG_TUSB_MCU  OPT_MCU_STM32H7
#endif

#ifndef CFG_TUSB_CONFIG_FILE
  #define CFG_TUSB_CONFIG_FILE "sys/stm32h745i/tinyusb/tusb_config.h"
#endif

// Now include the actual TinyUSB header
#include <tusb.h>

#ifdef __cplusplus
 }
#endif

#endif /* TUSB_WRAPPER_H_ */
