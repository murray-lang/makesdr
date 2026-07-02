#include "gpio/input/GpioInputLines.h"

ResultCode
GpioInputLines::configureLines(const Config::GpioInputLines::Fields& config)
{
  ResultCode rc = GpioLines::configureLines(config);
  if (rc != ResultCode::OK) return rc;

  m_debounce = config.debounce;
  m_activeHigh = config.activeHigh;
  m_rotaryEncoder = config.rotaryEncoder;

  return ResultCode::OK;
}