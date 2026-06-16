#ifndef USBD_WRAPPER_H_
#define USBD_WRAPPER_H_

#ifdef __cplusplus
 extern "C" {
#endif

   // Define MCU type for TinyUSB
#ifndef CFG_TUSB_MCU
#define CFG_TUSB_MCU  OPT_MCU_STM32H7
#endif

#define CFG_TUSB_CONFIG_FILE "stm32h745i/tinyusb/tusb_config.h"

   // Now include the actual TinyUSB header
#include <device/usbd.h>

#ifdef __cplusplus
 }
#endif

#endif /* USBD_WRAPPER_H_ */