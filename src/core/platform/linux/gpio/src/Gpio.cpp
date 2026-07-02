#include "gpio/service/Gpio.h"
#include <gpiod.h>

constexpr static auto defaultChipPath = "/dev/gpiochip0";

Gpio::Gpio() :
    m_pChip(nullptr)
{
}

Gpio::~Gpio()
{
  close();
}

bool
Gpio::isPresent()
{
  gpiod_chip* pChip = gpiod_chip_open(defaultChipPath);
  if (pChip == nullptr) {
    return false;
  }
  gpiod_chip_close(pChip);
  return true;
}

ResultCode
Gpio::open()
{
  m_pChip = gpiod_chip_open(defaultChipPath);
  if (m_pChip == nullptr) {
    return ResultCode::ERR_SETTING_CONTROL_GPIO_CHIP_OPEN;
  }
  return ResultCode::OK;
}

void
Gpio::close()
{
  if (m_pChip != nullptr) {
    gpiod_chip_close(m_pChip);
    m_pChip = nullptr;
  }
}

ResultCode
Gpio::requestInputs(const char * contextId, GpioInputLinesRequestVector& requests)
{
  if (!m_pChip) {
    return ResultCode::ERR_SETTING_CONTROL_GPIOD_CHIP_NOT_OPENED;
  }
  m_inputLinesSource.initialise(m_pChip, contextId);

  return m_inputLinesSource.requestLines(contextId, requests);
}

ResultCode
Gpio::requestOutputs(const char * contextId, GpioOutputLinesRequest* requests)
{
  if (!m_pChip) {
    return ResultCode::ERR_SETTING_CONTROL_GPIOD_CHIP_NOT_OPENED;
  }
  return m_outputLinesSource.requestLines(contextId, requests);
}

ResultCode Gpio::startInputs()
{
  return m_inputLinesSource.start();
}

void
Gpio::stopInputs()
{
  m_inputLinesSource.stop();
}