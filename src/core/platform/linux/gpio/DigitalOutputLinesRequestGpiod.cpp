#include "CrossPlatformTypes.h"
#include "settings/control/gpio/DigitalOutputLinesRequest.h"
#include "settings/control/gpio/DigitalOutputTypes.h"

DigitalOutputLinesRequest::DigitalOutputLinesRequest():
  m_gpio(Gpio::getInstance()),
  m_pChip(nullptr),
  m_pLineRequest(nullptr)
{
}

DigitalOutputLinesRequest::~DigitalOutputLinesRequest()
{
  DigitalOutputLinesRequest::release();
}

void
DigitalOutputLinesRequest::initialise(void* pChip, const char* consumer)
{
  m_pChip = pChip;
  m_consumer = consumer;
}

ResultCode
DigitalOutputLinesRequest::request(const char* contextId, const GpioLinesConfig& output)
{
  gpiod_line_config *lcfg = gpiod_line_config_new();
  if (lcfg == nullptr) {
    return ResultCode::ERR_SETTING_DIGITAL_OUTPUT_LINE_CONFIG;
  }

  gpiod_line_settings *ls = gpiod_line_settings_new();
  gpiod_line_settings_set_direction(ls, GPIOD_LINE_DIRECTION_OUTPUT);

  const GpioLineNumberVector& lineNos = output.getLines();
  gpiod_line_config_add_line_settings(lcfg, lineNos.data(), lineNos.size(), ls);
  gpiod_line_settings_free(ls);

  gpiod_request_config *rcfg = gpiod_request_config_new();
  gpiod_request_config_set_consumer(rcfg, contextId);

  gpiod_line_request *pLineRequest = gpiod_chip_request_lines(static_cast<gpiod_chip*>(m_pChip), rcfg, lcfg);
  gpiod_request_config_free(rcfg);
  gpiod_line_config_free(lcfg);
  if (pLineRequest == nullptr) {
    return ResultCode::ERR_SETTING_DIGITAL_OUTPUT_LINE_REQUEST_FAILED;
  }
  m_pLineRequest = pLineRequest;
  return ResultCode::OK;
}

// ResultCode request(const char * contextId, const DigitalOutputVariant& output)
// {
//
// }

ResultCode
DigitalOutputLinesRequest::setLineValue(uint32_t line, bool value)
{
  int rc = gpiod_line_request_set_value(static_cast<gpiod_line_request*>(m_pLineRequest), line, value ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
  if (rc < 0) {
    return ResultCode::ERR_SETTING_DIGITAL_OUTPUT_SET_LINE_FAILED;
  }
  return ResultCode::OK;
}

ResultCode
DigitalOutputLinesRequest::setLineValues(const GpioLineNumberVector& lines, const GpioOutputValuesVector& values)
{
  std::vector<enum gpiod_line_value> gpiodValues(values.size());
  for (size_t i = 0; i < values.size(); i++) {
    gpiodValues[i] = values[i] ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
  }
  int rc = gpiod_line_request_set_values_subset(
    static_cast<gpiod_line_request*>(m_pLineRequest),
    lines.size(),
    lines.data(),
    gpiodValues.data()
  );
  if (rc < 0) {
    return ResultCode::ERR_SETTING_DIGITAL_OUTPUT_SET_LINE_FAILED;
  }
  return ResultCode::OK;
}

void
DigitalOutputLinesRequest::release()
{
  if (m_pLineRequest != nullptr) {
    gpiod_line_request_release(static_cast<gpiod_line_request*>(m_pLineRequest));
    m_pLineRequest = nullptr;
  }
}

