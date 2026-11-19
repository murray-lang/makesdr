#include "stm32h745i/AdcConfig.h"


struct AdcConfig adcConfig = {
  .sampleRate = 0,
  .enableInput = true,
  .timer = {0},
  .timerMasterConfig = {0},
  .multimode = {0},
  .I = {
    .adc = {0},
    // .dma = {0},
    .channel = {0}
  },
  .Q = {
    .adc = {0},
    // .dma = {0},
    .channel = {0}
  }
};

struct AdcConfig* getAdcConfig()
{
  return &adcConfig;
}