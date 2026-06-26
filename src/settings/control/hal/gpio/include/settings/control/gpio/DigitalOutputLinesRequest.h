#pragma once
#include "base/Gpio.h"
#include "settings/control/gpio/base/GpioOutputLinesRequest.h"

class DigitalOutputLinesRequest : public GpioOutputLinesRequest
{
public:
  DigitalOutputLinesRequest();
  ~DigitalOutputLinesRequest() override;

  void initialise(void* gpioHandle, const char * context) override;
  void release() override;

  ResultCode request(const char* contextId, const GpioLinesConfig& output) override;

  ResultCode setLineValue(uint32_t line, bool value);
  ResultCode setLineValues(const GpioLineNumberVector& lines, const GpioOutputValuesVector& values);

protected:
  Gpio& m_gpio;
  GpioConsumerString m_consumer;

  void* m_pChip;
  void* m_pLineRequest;
};