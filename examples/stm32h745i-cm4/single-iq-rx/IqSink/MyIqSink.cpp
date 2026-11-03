#include "MyIqSink.h"

#include <stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h>


uint32_t
MyIqSink::sinkIq(ComplexSamplesBuffer& samples, uint32_t length)
{
  static uint32_t counter = 0;
  counter++;
  if (counter > 10000) {
    BSP_LED_Toggle(LED_RED);
    counter = 0;
  }
  return 0;
}
