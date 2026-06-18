
#include "testSettings.h"
#include "stm32h745i/app/cm4/config.h"
#include "stm32h745i/app/cm4/mpu_config.h"
#include "stm32h745i/app/support/safe_printf.h"
#include "stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h"

BandSettingsCache bandSettingsCache;

#ifdef __cplusplus
extern "C" {
#endif
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_4 || GPIO_Pin == GPIO_PIN_13)
  {
    /* Toggle LED2 */
    BSP_LED_Toggle(LED_RED);
  } else if (GPIO_Pin == GPIO_PIN_8) {
    BSP_LED_Toggle(LED_GREEN);
  }
}
#ifdef __cplusplus
}
#endif

extern "C" void EXTI3_IRQHandler(void);


int main()
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

  uint32_t vtor = SCB->VTOR;
  uint32_t* vectors = (uint32_t*)vtor;
  uint32_t exti3_vector = vectors[16 + EXTI3_IRQn];

  MX_GPIO_Init();

  MX_TIM6_Init();
  MX_DMA_Init();
  MX_FMC_Init();
  MX_SAI2_Init();
  MX_I2C4_Init();
  // MX_USART3_UART_Init();
  MX_DAC1_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();


  // __enable_irq();

  // RadioSettings radioSettings(exampleRadioSettingsPb, generalCoverageRadioMeta, bandSettingsCache);
  //
  // ResultCode rc = indirectUpdate(radioSettings);
  while (1) {}

}