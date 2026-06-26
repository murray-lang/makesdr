#include "stm32h745i/app/setup/line_readers.h"


class Stm32LineReader : public GpioLineReader
{
public:
  Stm32LineReader(GPIO_TypeDef* port, GpioLineMask pin) : m_port(port), m_lineMask(pin) {}

  [[nodiscard]] GpioLineValue readLine() const override
  {
    return HAL_GPIO_ReadPin(m_port, m_lineMask);
  }
  [[nodiscard]] GpioLineMask getLineMask() const override { return m_lineMask; }

private:
  GPIO_TypeDef* m_port;
  GpioLineMask m_lineMask;
};

Stm32LineReader pin1Reader(Digital_Input_1_GPIO_Port, GPIO_PIN_1);
Stm32LineReader pin3Reader(Digital_Input_3_GPIO_Port, GPIO_PIN_3);
Stm32LineReader pin4Reader(Digital_Input_4_GPIO_Port, GPIO_PIN_4);
Stm32LineReader pin6Reader(Digital_Input_6_GPIO_Port, GPIO_PIN_6);
Stm32LineReader pin8Reader(Digital_Input_8_GPIO_Port, GPIO_PIN_8);
Stm32LineReader pin13Reader(Digital_Input_13_GPIO_Port, GPIO_PIN_13);
Stm32LineReader pin14Reader(Digital_Input_14_GPIO_Port, GPIO_PIN_14);
Stm32LineReader pin15Reader(Digital_Input_15_GPIO_Port, GPIO_PIN_15);

GpioLineReader* lineReaders[NUM_LINE_READERS] = {
  nullptr,
  &pin1Reader,
  nullptr,
  &pin3Reader,
  &pin4Reader,
  nullptr,
  &pin6Reader,
  nullptr,
  &pin8Reader,
  nullptr,
  nullptr,
  nullptr,
  nullptr,
  &pin13Reader,
  &pin14Reader,
  &pin15Reader
};

// GpioLineReader* request_line_reader(GpioLineMask mask)
// {
//   uint16_t line = __builtin_ctz(mask);
//   if (line < 16) {
//     return lineReaders[line];
//   }
//   return nullptr;
// }