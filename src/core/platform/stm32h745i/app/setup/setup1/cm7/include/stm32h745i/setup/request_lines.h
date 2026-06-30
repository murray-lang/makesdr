#ifndef STM32_CM7_REQUEST_LINES_H
#define STM32_CM7_REQUEST_LINES_H
#include <stm32h745i/drivers/hal/stm32h7xx_hal.h>

extern bool request_input_line(uint16_t line, GPIO_TypeDef** port, uint16_t* pin);

#endif // STM32_CM7_REQUEST_LINES_H