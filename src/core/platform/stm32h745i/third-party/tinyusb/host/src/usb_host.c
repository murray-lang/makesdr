#include <stdio.h>
#include <tinyusb/host/tusb.h>
#include <stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h>

/**
  * @brief  Initializes USB Device as HOST
  * @param  None
  * @retval None
  */
void USB_Host_Init(void)
{
  // printf("[USB]:\tInitializing USB host hardware...\r\n");

  /* Enable USB OTG peripheral clock */
  __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
  // __HAL_RCC_USB_OTG_HS_CLK_ENABLE();

  /* Enable USB voltage level detector */
  HAL_PWREx_EnableUSBVoltageDetector();

  // HAL_StatusTypeDef hr = HAL_PWREx_EnableUSBReg();
  // if (hr != HAL_OK) {
  //   printf("[USB]:\tFailed to enable USB regulator: %u\r\n", hr);
  // }
  // Do NOT enable the internal USB regulator (HAL_PWREx_EnableUSBReg): on the
  // STM32H745I-DISCO VDD33USB is supplied externally, and setting USBREGEN keeps
  // USB33RDY low, leaving the FS PHY unpowered (D+/D- never seen, no OTG_FS IRQ).
  uint32_t t0 = HAL_GetTick();
  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_USB33RDY) && (HAL_GetTick() - t0) < 10) {}
  if (!__HAL_PWR_GET_FLAG(PWR_FLAG_USB33RDY)) {
    // printf("[USB]:\tWARNING: VDD33USB not ready (USB33RDY=0)\r\n");
  }

  // Optional VBUS power cycle on the OTG port (disabled for testing). Added while
  // chasing a hub that never reported its downstream devices after an MCU reset;
  // the real causes turned out to be a starved USB host thread and a faulty cable,
  // so this may be unnecessary. Re-enable if a hub misbehaves after reset.
  // NB: HAL_Delay() is only accurate if HAL_InitTick() was re-run after the CM7
  // raised the clocks (see main()).
  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
  // HAL_Delay(500);

  // Turn on power to USB OTG
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

  // Force host mode and disable hardware VBUS sensing
  // USB_OTG_FS->GUSBCFG |= USB_OTG_GUSBCFG_FHMOD; // Force Host Mode
  // USB_OTG_FS->GCCFG |= USB_OTG_GCCFG_VBDEN;    // Disable VBUS sensing



  /* Enable USB interrupt in NVIC */
  // HAL_NVIC_SetPriority(OTG_HS_IRQn, 6, 0);
  // HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
  HAL_NVIC_SetPriority(OTG_FS_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(OTG_FS_IRQn);

  HAL_Delay(100);
  // /* Check VBUS status */
  GPIO_PinState vbus = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
  // printf("[USB]:\tVBUS detected: %s\r\n", vbus == GPIO_PIN_SET ? "YES" : "NO");

  // printf("[USB]:\tIRQ enabled: %d\r\n", NVIC_GetEnableIRQ(OTG_HS_IRQn));
  // printf("[USB]:\tIRQ pending: %d\r\n", NVIC_GetPendingIRQ(OTG_HS_IRQn));
  // printf("[USB]:\tIRQ active: %d\r\n", NVIC_GetActive(OTG_HS_IRQn));
  // printf("[USB]:\tIRQ enabled: %d\r\n", NVIC_GetEnableIRQ(OTG_FS_IRQn));
  // printf("[USB]:\tIRQ pending: %d\r\n", NVIC_GetPendingIRQ(OTG_FS_IRQn));
  // printf("[USB]:\tIRQ active: %d\r\n", NVIC_GetActive(OTG_FS_IRQn));

  /* Initialize TinyUSB - rhport 0 = USB_OTG_FS */
  tusb_rhport_init_t dev_init = {
    .role = TUSB_ROLE_HOST,
    .speed = TUSB_SPEED_FULL
  };

  // printf("[USB]:\tInitializing TinyUSB host...\r\n");
  if (!tusb_init(BOARD_TUH_RHPORT, &dev_init)) {
    // printf("[USB]:\tERROR: tusb_init failed!\r\n");
    return;
  }
  // printf("[USB]:\tTinyUSB host initialized successfully\r\n");
}