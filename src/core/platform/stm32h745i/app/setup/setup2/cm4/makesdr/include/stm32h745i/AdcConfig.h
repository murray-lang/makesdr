#ifndef STM32H745I_ADC_CONFIG_H
#define STM32H745I_ADC_CONFIG_H

#include <stdbool.h>
#include <stm32h745i/drivers/hal/stm32h7xx_hal.h>

struct AdcChannelConfig
{
  ADC_HandleTypeDef adc;   // ADC for I channel (e.g., ADC1)

  // DMA configuration
  DMA_HandleTypeDef dma;
  ADC_ChannelConfTypeDef channel;
};

struct AdcConfig
{
  uint32_t sampleRate;        // Sample rate in Hz (e.g., 48000, 96000, 192000)
  bool enableInput;           // Enable ADC input

  // Timer for sampling trigger
  TIM_HandleTypeDef timer;
  TIM_MasterConfigTypeDef timerMasterConfig;

  ADC_MultiModeTypeDef multimode;

  struct AdcChannelConfig I;
  struct AdcChannelConfig Q;
};

#ifdef __cplusplus
extern "C" {
#endif

extern struct AdcConfig* getAdcConfig();

#ifdef __cplusplus
  }
#endif

#endif // STM32H745I_ADC_CONFIG_H