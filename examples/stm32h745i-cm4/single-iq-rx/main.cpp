
#include "stm32h745i/config.h"
#include "stm32h745i/mpu_config.h"
#include "stm32h745i/app/support/safe_printf.h"
#include "stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h"

#include <radios/iq/SingleIqRxRadio.h>
#include <settings/model/radio/iq/SingleIqRxSettings.h>

#include <settings/model/meta/band/mostBandCategories.h>
#include <settings/model/meta/mode/basicModes.h>
#include <test/settings/testRadioSettings.h>

#include "IqSink/MyIqSink.h"
// #include <stm32h745i/setup/AdcConfig.h>

#include "config/radioConfig.h"
#include "tinyusb/host/usb_host.h"

// #include "audio/audioLoopbackTask.h" // Out of the build: see single-iq-rx.cmake

BandCategoryList bandsByCategory(mostBandCategories);
ModeList modeList(basicModes);
SingleIqRxSettings::Cache bandSettingsCache;

//
static Config::Radio::Fields radioConfig;
//
SingleIqRxSettings settings(testRadioSettingsPayloadPb, true);
//
Gpio& gpio = Gpio::getInstance();


#ifdef __cplusplus
extern "C" {
#endif

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  uint32_t ticks = HAL_GetTick();
  // if (GPIO_Pin == Digital_Input_4_Pin) BSP_LED_Toggle(LED_GREEN);
  // if (GPIO_Pin == Digital_Input_8_Pin) BSP_LED_Toggle(LED_RED);
  gpio.handlePinTransition(GPIO_Pin, ticks);
}

// void OTG_FS_IRQHandler(void) {
//   // BSP_LED_Toggle(LED_RED);
//   tuh_int_handler(BOARD_TUH_RHPORT, true);  // or hcd_int_handler()
// }

#ifdef __cplusplus
}
#endif

// ADC IQ arrives by DMA: AdcIqSource registers its own HAL ADC callbacks, so none are defined here.

static void prvRadioTask( void *pvParameters );

extern "C" int main()
{
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  HAL_StatusTypeDef halRc = HAL_Init();
  if (halRc != HAL_OK) {
    Error_Handler();
  }
  MPU_Config_Shared();

  /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();

  /* Activate HSEM notification for Cortex-M4*/
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

  /*
    Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep)
  */
  HAL_PWREx_ClearPendingEvent();
  HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);


  /* Clear HSEM flag */
  __HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
  SystemCoreClockUpdate();
  // HAL_Init() set SysTick for the 64 MHz reset clock; the CM7 has since raised it
  // to 200 MHz, so re-time the HAL tick or every pre-scheduler HAL_Delay() runs ~3x
  // short. (FreeRTOS reprograms SysTick itself when the scheduler starts.)
  HAL_InitTick(uwTickPrio);

  UART_Config();  /* USART3 on PB10/PB11 - no conflict with USB on PA11/PA12 */
  SAFE_PRINTF("[CM4]\t UART_Config() returned\r\n");
  // uint32_t vtor = SCB->VTOR;
  // uint32_t* vectors = (uint32_t*)vtor;
  // uint32_t exti3_vector = vectors[16 + EXTI3_IRQn];

  MX_GPIO_Init();
  // HAL_GPIO_WritePin(USB_OTG_Power_GPIO_Port, USB_OTG_Power_Pin, GPIO_PIN_SET);
  MX_DMA_Init();
  MX_FMC_Init();
  // MX_SAI1_Init();
  // MX_I2C4_Init(); //BSP initialises this
  USB_Host_Init();


  SAFE_PRINTF("[CM4]\tCalling xTaskCreate()\r\n");
  BaseType_t taskRc = xTaskCreate( prvRadioTask, "RX", 3072, nullptr, tskIDLE_PRIORITY, nullptr );
  // BaseType_t taskRc = xTaskCreate( vAudioLoopbackTask, "RX", 3072, nullptr, configMAX_PRIORITIES - 2, nullptr );
  if (taskRc == pdPASS) {
    SAFE_PRINTF("[CM4]\txTaskCreate() succeeded\r\n");
  } else {
    SAFE_PRINTF("[CM4]\txTaskCreate() returned: %ld\r\n", taskRc);
  }

  vTaskStartScheduler();
  while (1) {
    // HAL_Delay(1000);
  }

}

static void prvRadioTask( void *pvParameters )
{
  static SingleIqRxRadio radio(bandsByCategory, modeList, bandSettingsCache);
  SAFE_PRINTF("[CM4]\t In prvRadioTask()\r\n");
  createRadioConfig(radioConfig);
  ResultCode rc = radio.configure(radioConfig);
  if (rc != ResultCode::OK) {
    SAFE_PRINTF("[CM4]\t radio.configure() returned %d\r\n", static_cast<int>(rc));
  } else {
    SAFE_PRINTF("[CM4]\t radio.configure() successful\r\n");
  }

  radio.setSettings(settings);

  rc = radio.start();
  if (rc != ResultCode::OK) {
    radio.stop();
    SAFE_PRINTF("[CM4]\t radio.start() returned %d\r\n", static_cast<int>(rc));
  } else {
    SAFE_PRINTF("[CM4]\t radio.start() successful\r\n");
  }

  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(10);
  while (1) {
    // vTaskDelay(pdMS_TO_TICKS(1000)); // Sleep to avoid wasting CPU
    // if (rc == ResultCode::OK) {
    //   int64_t& centerFrequency = radioSettings.body().active_bands.band_1.pipeline_a.base.rf.centre_frequency.value;
    //   SAFE_PRINTF("[CM4]\t Centre frequency %ld\r\n", static_cast<int32_t>(centerFrequency));
    // }
    // BSP_LED_Toggle(LED_GREEN);
    //if (rc == ResultCode::OK) {
    // tuh_task_ext(xFrequency, false);
    // tuh_task();
    // }


    // Wait for next cycle
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

// void vApplicationMallocFailedHook( void )
// {
//   /* Total heap exhaustion or memory fragmentation occurred */
//
//   /* 1. Disable interrupts to stabilize the system */
//   __disable_irq();
//
//   /* 2. Force a breakpoint for debugging, or log the failure */
//   // printf("Error: pvPortMalloc failed due to insufficient heap!\n");
//   BSP_LED_On(LED_RED);
//   /* 3. Enter a safe state / infinite loop or trigger a system reset */
//   while( 1 )
//   {
//     // Blink an error LED or wait for a watchdog reset
//
//   }
// }

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
  /* The task specified by xTask has exceeded its stack size */

  /* 1. Prevent context switching and disable interrupts */
  // __disable_irq();

  /* 2. Extract the name of the culprit task (pcTaskName contains this string) */
  (void) xTask;       /* Avoid compiler warning if unused */
  // (void) pcTaskName;  /* Avoid compiler warning if unused */

  // SAFE_PRINTF("Stack overflow detected: %s\r\n", pcTaskName);
  BSP_LED_On(LED_RED);

  /* 3. Enter a safe state, signal a fault condition, or force a reset */
  while( 1 )
  {
    /* Turn on a red fault LED or wait for a watchdog reset */
  }
}
