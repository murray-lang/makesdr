#include "stm32h745i/gpio/GpioOutputs.h"


constexpr uint16_t NUM_LINE_WRITERS = 16;

class Stm32LineWriter
{
public:
  Stm32LineWriter(GPIO_TypeDef* port, GpioLineMask pin) : m_port(port), m_lineMask(pin) {}

  void writeLine(GpioLineValue value) const
  {
    HAL_GPIO_WritePin(m_port, m_lineMask, value > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  }
  [[nodiscard]] GpioLineMask getLineMask() const { return m_lineMask; }

private:
  GPIO_TypeDef* m_port;
  GpioLineMask m_lineMask;
};

Stm32LineWriter pin2Writer(Digital_Output_2_GPIO_Port, GPIO_PIN_2);
// Stm32LineWriter pin3Writer(Digital_Output_3_GPIO_Port, GPIO_PIN_3);
Stm32LineWriter pin4Writer(Digital_Output_4_GPIO_Port, GPIO_PIN_4);
Stm32LineWriter pin8Writer(Digital_Output_8_GPIO_Port, GPIO_PIN_8);
// Stm32LineWriter pin11Writer(Digital_Output_11_GPIO_Port, GPIO_PIN_11); //Removed due to clash with USB OTG Host mode
// Stm32LineWriter pin15Writer(Digital_Output_15_GPIO_Port, GPIO_PIN_15); //Removed due to clash with USB OTG Host mode

Stm32LineWriter* lineWriters[NUM_LINE_WRITERS] = {
  nullptr,
  nullptr,
  &pin2Writer,
  nullptr,
  &pin4Writer,
  nullptr,
  nullptr,
  nullptr,
  &pin8Writer,
  nullptr,
  nullptr,
  nullptr, //&pin11Writer, //Removed due to clash with USB OTG Host mode
  nullptr,
  nullptr,
  nullptr,
  nullptr, //&pin15Writer, //Removed due to clash with USB OTG Host mode
};


ResultCode
gpioWriteLine(GpioLineMask mask, const GpioLineValue& value)
{
  if (!isGpioOutputLineAvailable(mask)) return ResultCode::ERR_GPIO_OUTPUT_LINE_NOT_AVAILABLE;

  uint16_t line = __builtin_ctz(mask);
  lineWriters[line]->writeLine(value);
  return ResultCode::OK;
}

bool
isGpioOutputLineAvailable(GpioLineMask mask)
{
  uint16_t line = __builtin_ctz(mask);
  return line < NUM_LINE_WRITERS && lineWriters[line] != nullptr;
}