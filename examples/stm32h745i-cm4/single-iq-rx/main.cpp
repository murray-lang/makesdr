
#include "stm32h745i/setup/config.h"
#include "stm32h745i/setup/mpu_config.h"
#include "stm32h745i/app/support/safe_printf.h"
#include "stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h"

#include "IqSink/MyIqSink.h"
#include <stm32h745i/setup/AdcConfig.h>

#include <iq/source/AdcIqSource.h>

MyIqSink g_iqSink;

AdcIqSource g_iqSource(&g_iqSink);

// Choose ADC mode: uncomment one
// #define USE_ADC_DMA_MODE
#define USE_ADC_INTERRUPT_MODE

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_ADC_DMA_MODE
  /**
   * ADC conversion complete callback (DMA mode)
   * Called when DMA has filled the second half of the buffer
   */
  void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
  {
    if (hadc->Instance == ADC1)
    {
      g_iqSource.onAdcConversionComplete();
    }
  }

  /**
   * ADC conversion half complete callback (DMA mode)
   * Called when DMA has filled the first half of the buffer
   */
  void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
  {
    if (hadc->Instance == ADC1)
    {
      g_iqSource.onAdcConversionHalfComplete();
    }
  }
#endif

#ifdef USE_ADC_INTERRUPT_MODE
  /**
   * ADC conversion complete callback (Interrupt mode)
   * Called when an ADC conversion completes
   * In dual mode, reads both ADC1 (I) and ADC2 (Q) values
   */
  void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
  {
    AdcConfig* adcConfig = getAdcConfig();
    // In dual simultaneous mode, read both ADC values
    uint16_t valueI = HAL_ADC_GetValue(&adcConfig->I.adc);
    uint16_t valueQ = HAL_ADC_GetValue(&adcConfig->Q.adc);

    g_iqSource.onAdcDualConversion(valueI, valueQ);
    // static uint32_t counter = 0;
    // counter++;
    // if (counter > 10000) {
    //   BSP_LED_Toggle(LED_RED);
    //   counter = 0;
    // }

  }
#endif

  void HAL_ADC_ErrorCallback(ADC_HandleTypeDef* hadc)
  {
    // BSP_LED_On(LED_RED);
  }

#ifdef __cplusplus
}
#endif

static void prvAdcTask( void *pvParameters );

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

  UART_Config();  /* USART3 on PB10/PB11 - no conflict with USB on PA11/PA12 */
  SAFE_PRINTF("[CM4]\t UART_Config() returned\r\n");
  // uint32_t vtor = SCB->VTOR;
  // uint32_t* vectors = (uint32_t*)vtor;
  // uint32_t exti3_vector = vectors[16 + EXTI3_IRQn];

  MX_GPIO_Init();
  // HAL_GPIO_WritePin(USB_OTG_Power_GPIO_Port, USB_OTG_Power_Pin, GPIO_PIN_SET);
  MX_DMA_Init();
  MX_FMC_Init();
  MX_SAI2_Init();
  MX_I2C4_Init();

  BaseType_t taskRc = xTaskCreate( prvAdcTask, "ADC", configMINIMAL_STACK_SIZE*4, nullptr, tskIDLE_PRIORITY, nullptr );
  if (taskRc == pdPASS) {
    SAFE_PRINTF("[CM4]\txTaskCreate() succeeded\r\n");
  } else {
    SAFE_PRINTF("[CM4]\txTaskCreate() returned: %ld", taskRc);
  }

  vTaskStartScheduler();
  while (1) {
    // HAL_Delay(1000);
  }

}

static void prvAdcTask( void *pvParameters )
{
  ResultCode rc = g_iqSource.initialise();
  if (rc == ResultCode::OK) {
    rc = g_iqSource.start(0);
    if (rc == ResultCode::OK) {
      BSP_LED_On(LED_GREEN);
    } else {
      BSP_LED_On(LED_RED);
      SAFE_PRINTF("[CM4]\t g_iqSource.start() returned %d\r\n", static_cast<int>(rc));
    }
  } else {
    BSP_LED_On(LED_RED);
    SAFE_PRINTF("[CM4]\t g_iqSource.initialise() returned %d\r\n", static_cast<int>(rc));
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
