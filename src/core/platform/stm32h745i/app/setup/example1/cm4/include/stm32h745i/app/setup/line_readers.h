#ifndef STM32_CM4_REQUEST_LINES_H
#define STM32_CM4_REQUEST_LINES_H

#include "config.h"

#include "gpio/base/GpioLineReader.h"

// extern GpioLineReader* request_line_reader(GpioLineMask mask);
constexpr uint16_t NUM_LINE_READERS = 16;
extern GpioLineReader* lineReaders[NUM_LINE_READERS];

#endif // STM32_CM4_REQUEST_LINES_H