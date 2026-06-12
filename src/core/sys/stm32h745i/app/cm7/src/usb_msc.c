#include "sys/stm32h745i/app/cm7/usb_msc.h"
#include <sys/stm32h745i/app/common/misc_utils.h>
#include <sys/stm32h745i/tinyusb/tusb.h>
#include <sys/stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h>

/**
  * @brief  Initializes USB Device as MSC
  * @param  None
  * @retval None
  */
void USB_Device_Init(void)
{
  SAFE_PRINTF("[USB]:\tInitializing USB peripheral hardware...\r\n");

  /* Enable USB OTG FS peripheral clock */
  __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

  /* Enable USB voltage level detector */
  HAL_PWREx_EnableUSBVoltageDetector();

  /* Check VBUS status */
  GPIO_PinState vbus = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
  SAFE_PRINTF("[USB]:\tVBUS detected: %s\r\n", vbus == GPIO_PIN_SET ? "YES" : "NO");

  /* Enable USB interrupt in NVIC */
  HAL_NVIC_SetPriority(OTG_FS_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(OTG_FS_IRQn);

  /* Initialize TinyUSB - rhport 0 = USB_OTG_FS */
  tusb_rhport_init_t dev_init = {
    .role = TUSB_ROLE_DEVICE,
    .speed = TUSB_SPEED_AUTO
  };

  SAFE_PRINTF("[USB]:\tInitializing TinyUSB...\r\n");
  if (!tusb_init(BOARD_TUD_RHPORT, &dev_init)) {
    SAFE_PRINTF("[USB]:\tERROR: tusb_init failed!\r\n");
    return;
  }
  SAFE_PRINTF("[USB]:\tTinyUSB initialized successfully - ready for enumeration\r\n");
}

//--------------------------------------------------------------------+
// TinyUSB Device Callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) {
  BSP_LED_On(LED_GREEN);
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
  BSP_LED_Off(LED_GREEN);
}

// Invoked when usb bus is suspended
void tud_suspend_cb(bool remote_wakeup_en) {
  (void) remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
}
