#include "gpio/service/Gpio.h"



ResultCode
Gpio::requestInputs(const char * contextId, GpioInputLinesRequestVector& requests)
{
  return m_inputLinesSource.requestLines(contextId, requests);
}

ResultCode
Gpio::requestOutputs(const char * contextId, GpioOutputLinesRequest* requests)
{
  return m_outputLinesSource.requestLines(contextId, requests);
}

ResultCode Gpio::startInputs()
{
  return m_inputLinesSource.start(configMINIMAL_STACK_SIZE*5, tskIDLE_PRIORITY, "gpio");
}

void
Gpio::stopInputs()
{
  m_inputLinesSource.stop();
}