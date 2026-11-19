#include "stm32h745i/config.h"
#include "stm32h745i/app/support/safe_printf.h"
// #include "tusb.h"
// #include "stm32h7xx_hal_adc.h"
// #include "stm32h7xx_hal_dac.h"
// #include "stm32h7xx_hal_sai.h"

UART_HandleTypeDef     UartHandle;

void UART_Config(void)
{

  UartHandle.Instance        = USARTx;
  UartHandle.Init.BaudRate   = 115200;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

  // Don't deinit - CM7 already configured the hardware
  // HAL_UART_DeInit(&UartHandle);
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    // Error_Handler();
    while(1) {
      BSP_LED_Toggle(LED_RED);
      HAL_Delay(100);
    }
  }
  // BSP_LED_Off(LED_RED);  // Signal success
}

PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFFF);
  return ch;
}

void Error_Handler(void)
{
  SAFE_PRINTF("[CM4]:\tError!! \n");
  __disable_irq();
  while(1)
  {
  }
}

enum ResultCode ADC1_Init(struct AdcChannelConfig* adcChannelConfig, ADC_MultiModeTypeDef* multimode)
{
  // ADC_HandleTypeDef hadc1 = {0};
  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  // ADC_MultiModeTypeDef multimode = {0};
  // ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  adcChannelConfig->adc.Instance = ADC1;
  adcChannelConfig->adc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
  adcChannelConfig->adc.Init.Resolution = ADC_RESOLUTION_16B;
  adcChannelConfig->adc.Init.ScanConvMode = ADC_SCAN_DISABLE;
  adcChannelConfig->adc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  adcChannelConfig->adc.Init.LowPowerAutoWait = DISABLE;
  adcChannelConfig->adc.Init.ContinuousConvMode = DISABLE;
  adcChannelConfig->adc.Init.NbrOfConversion = 1;
  adcChannelConfig->adc.Init.DiscontinuousConvMode = DISABLE;
  adcChannelConfig->adc.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T6_TRGO;
  adcChannelConfig->adc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  adcChannelConfig->adc.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  adcChannelConfig->adc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  adcChannelConfig->adc.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  adcChannelConfig->adc.Init.OversamplingMode = DISABLE;
  adcChannelConfig->adc.Init.Oversampling.Ratio = 1;
  HAL_StatusTypeDef rc = HAL_ADC_Init(&adcChannelConfig->adc);
  if (rc != HAL_OK)
  {
    return ERR_ADC_INIT;
  }

  /** Configure the ADC multi-mode
  */
  multimode->Mode = ADC_DUALMODE_REGSIMULT;
  multimode->DualModeData = ADC_DUALMODEDATAFORMAT_32_10_BITS;
  multimode->TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_1CYCLE;
  rc = HAL_ADCEx_MultiModeConfigChannel(&adcChannelConfig->adc, multimode);
  if (rc != HAL_OK)
  {
    return ERR_ADC_INIT;
  }

  /** Configure Regular Channel
  */
  adcChannelConfig->channel.Channel = ADC_CHANNEL_1;
  adcChannelConfig->channel.Rank = ADC_REGULAR_RANK_1;
  adcChannelConfig->channel.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  adcChannelConfig->channel.SingleDiff = ADC_SINGLE_ENDED;
  adcChannelConfig->channel.OffsetNumber = ADC_OFFSET_NONE;
  adcChannelConfig->channel.Offset = 0;
  adcChannelConfig->channel.OffsetSignedSaturation = DISABLE;
  rc = HAL_ADC_ConfigChannel(&adcChannelConfig->adc, &adcChannelConfig->channel);
  if (rc != HAL_OK)
  {
    return ERR_ADC_INIT;
  }
  HAL_NVIC_SetPriority(ADC_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(ADC_IRQn);
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */
  return OK;
}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
enum ResultCode ADC2_Init(struct AdcChannelConfig* adcChannelConfig)
{
  // ADC_HandleTypeDef hadc2 = {0};
  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  // ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  adcChannelConfig->adc.Instance = ADC2;
  adcChannelConfig->adc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
  adcChannelConfig->adc.Init.Resolution = ADC_RESOLUTION_16B;
  adcChannelConfig->adc.Init.ScanConvMode = ADC_SCAN_DISABLE;
  adcChannelConfig->adc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  adcChannelConfig->adc.Init.LowPowerAutoWait = DISABLE;
  adcChannelConfig->adc.Init.ContinuousConvMode = DISABLE;
  adcChannelConfig->adc.Init.NbrOfConversion = 1;
  adcChannelConfig->adc.Init.DiscontinuousConvMode = DISABLE;
  adcChannelConfig->adc.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  adcChannelConfig->adc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  adcChannelConfig->adc.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  adcChannelConfig->adc.Init.OversamplingMode = DISABLE;
  adcChannelConfig->adc.Init.Oversampling.Ratio = 1;
  if (HAL_ADC_Init(&adcChannelConfig->adc) != HAL_OK)
  {
    return ERR_ADC_INIT;
  }

  /** Configure Regular Channel
  */
  adcChannelConfig->channel.Channel = ADC_CHANNEL_3;
  adcChannelConfig->channel.Rank = ADC_REGULAR_RANK_1;
  adcChannelConfig->channel.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  adcChannelConfig->channel.SingleDiff = ADC_SINGLE_ENDED;
  adcChannelConfig->channel.OffsetNumber = ADC_OFFSET_NONE;
  adcChannelConfig->channel.Offset = 0;
  adcChannelConfig->channel.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&adcChannelConfig->adc, &adcChannelConfig->channel) != HAL_OK)
  {
    return ERR_ADC_INIT;
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */
  return OK;
}
