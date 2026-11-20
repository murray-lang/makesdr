/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stm32h745i/drivers/cmsis/device//stm32h745xx.h>
#include <stm32h745i/drivers/bsp/disco/stm32h745i_discovery_qspi.h>
// #include "stm32h745i/drivers/bsp/disco/stm32h745i_discovery_sdram.h"
// #include <stm32h745i/drivers/bsp/disco/stm32h745i_discovery_mmc.h>

#include <lvgl/lvgl.h>
#include <lvgl_port_lcd.h>
#include <lvgl_port_touchpad.h>
#include <ui/lvgl/RadioDisplay.h>
#include <demos/lv_demos.h>


#include <stm32h745i/app/support/safe_printf.h>
#include <stm32h745i/mpu_config.h>
#include <stm32h745i/config.h>

#include <transport/stm32h745i/globalStm32H745SingleIqRxRings.h>


#include <stdio.h>

#include <string.h>
#include <stm32h745i/app/support/usb_manager.h>
// #include <tinyusb/device/usbd.h>

#include "stm32h745i/client/RadioClient.h"
#include "stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h"
#include "tinyusb/device/usb_device.h"
// #include "transport/freertos/client/IqCapableClientTransportT.h"
// #include <ArduinoJson.h>

#ifdef __cplusplus
 extern "C" {
#endif


#include "FreeRTOS.h"
#include "task.h"

#ifdef __cplusplus
 }
#endif



// #define LVGL_TASK_PRIORITY			( configMAX_PRIORITIES - 2 )
#define LVGL_TASK_PRIORITY			( 3 )

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif


static RadioDisplay radioDisplay;

// Passes the client's settings to the display. Both are on the LVGL task.
class DisplaySettingsListener : public ISettingsListener
{
public:
  void radioSettingsChanged(const RadioClient::RadioSettings& state, const RadioClient::RadioSettings& changes) override
  {
    radioDisplay.handleRadioSettingsChanged<RadioClient::RadioSettings>(state, changes);
  }
};
static DisplaySettingsListener displaySettingsListener;

static void prvLvglTask( void *pvParameters );

extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
  (void)xTask;
  (void)pcTaskName;
  __disable_irq();
  while (1) {
  }
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  MPU_Config();
  createStm32H745SingleIqRxRings();
  CPU_CACHE_Enable();

  /* Boot CM4 core */
  HAL_RCCEx_EnableBootCore(RCC_BOOT_C2);

  /* Immediately take HSEM before CM4 gets to STOP mode */
  __HAL_RCC_HSEM_CLK_ENABLE();
  HAL_HSEM_FastTake(HSEM_ID_0);  // Take it ASAP

  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  // Note that the CM4 initialises GPIO, and needs to finish that before this CM7
  // can use the USB
  // The CM4's Reset_Handler zeroes its .bss (~200K with the radio) from
  // flash at the 64MHz reset clock before reaching main(), which took
  // longer than ST's default 0xFFFF iterations.
  int32_t timeout = 0x00FFFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0)) {}

  if ( timeout < 0 )
  {
    Error_Handler();
  }
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
  PeriphCommonClock_Config();
  SystemCoreClockUpdate();
  __HAL_RCC_HSEM_CLK_ENABLE(); // Again after SystemCoreClockUpdate()

  MX_GPIO_Init();

  MX_QUADSPI_Init();

  UART_Config();

  // MX_GPIO_Init();
  MX_DMA_Init();
  MX_FMC_Init();


  SAFE_PRINTF("[CM7]:\tAbout to wake up CM4\r\n");
  LOCK_HSEM(HSEM_ID_0);
  UNLOCK_HSEM(HSEM_ID_0);// This signals the CM4 to wake up

  static RadioClient radioClient;

  // The one task for LVGL, the radio client and its transport
  BaseType_t rc = xTaskCreate( prvLvglTask, "LVGL", 2048, &radioClient, LVGL_TASK_PRIORITY, NULL );
  if (rc != pdPASS) {
    Error_Handler();
  }
  vTaskStartScheduler();

  /* Infinite loop (never reached because vTaskStartScheduler() never returns)*/
  /* USER CODE BEGIN WHILE */
	while (1)
	{
	  // HAL_Delay(500);
	  //vTaskDelay(pdMS_TO_TICKS(500));
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

static void prvLvglTask( void *pvParameters )
{
  // SAFE_PRINTF("[CM7]:\tInitializing LVGL and LCD...\r\n");
  lv_init();
  lv_tick_set_cb(HAL_GetTick);
  LCD_init();
  // SAFE_PRINTF("[CM7]:\tLCD initialized\r\n");

  touchpad_init();

  RadioClient* radioClient = (RadioClient *)pvParameters;

  radioDisplay.initialise(radioClient);
  radioDisplay.start();

  radioClient->connectSettingsListener(&displaySettingsListener);
  radioClient->connectSpectrumSink(&radioDisplay);
  radioClient->connectRxMeteringSink(&radioDisplay);

  // This task now hosts the transport: messages are delivered to the client
  // and the display from radioClient.waitAndDispatch() below.
  ResultCode rc = radioClient->start();
  SAFE_PRINTF("[CM7]:\tradioClient.start() returned %d\r\n", rc);
  if (rc == ResultCode::OK) {
    // Returns at once; the replies advance it
    radioClient->requestAll();
  }

  for (;;) {
    uint32_t idleMs = lv_timer_handler();
    if (idleMs == LV_NO_TIMER_READY || idleMs > 10) {
      idleMs = 10;
    }
    // Must block, so lower priority tasks can run. Wakes early for messages.
    const TickType_t ticks = pdMS_TO_TICKS(idleMs);
    radioClient->waitAndDispatch(ticks > 0 ? ticks : 1);
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

