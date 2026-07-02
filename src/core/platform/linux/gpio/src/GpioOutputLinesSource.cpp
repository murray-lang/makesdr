#include "gpio/service/GpioOutputLinesSource.h"


GpioOutputLinesSource::GpioOutputLinesSource()
  : m_lineWriter(GpioLineWriter::create<GpioOutputLinesSource, &GpioOutputLinesSource::writeLine>(*this))
{

}

ResultCode
GpioOutputLinesSource::writeLine(GpioLineMask mask, const GpioLineValue& value)
{
  unsigned int line = __builtin_ctz(mask);
  int rc = gpiod_line_request_set_value(m_pLineRequest, line, value > 0 ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
  if (rc < 0) {
    return ResultCode::ERR_SETTING_DIGITAL_OUTPUT_SET_LINE_FAILED;
  }
  return ResultCode::OK;
}

ResultCode
GpioOutputLinesSource::requestLines(const char* context, GpioOutputLinesRequest* request)
{
  gpiod_line_config *lcfg = gpiod_line_config_new();
  if (lcfg == nullptr) {
    return ResultCode::ERR_SETTING_DIGITAL_OUTPUT_LINE_CONFIG;
  }

  gpiod_line_settings *ls = gpiod_line_settings_new();
  gpiod_line_settings_set_direction(ls, GPIOD_LINE_DIRECTION_OUTPUT);

  GpioLineNumberVector lineNos;
  GpioLineMask requestedLineMask = request->config.getLines();
  getLineNumbers(requestedLineMask, lineNos);

  gpiod_line_config_add_line_settings(lcfg, lineNos.data(), lineNos.size(), ls);
  gpiod_line_settings_free(ls);

  gpiod_request_config *rcfg = gpiod_request_config_new();
  gpiod_request_config_set_consumer(rcfg, context);

  gpiod_line_request *pLineRequest = gpiod_chip_request_lines(m_pChip, rcfg, lcfg);
  gpiod_request_config_free(rcfg);
  gpiod_line_config_free(lcfg);
  if (pLineRequest == nullptr) {
    return ResultCode::ERR_SETTING_DIGITAL_OUTPUT_LINE_REQUEST_FAILED;
  }
  request->lineWriter = m_lineWriter;
  m_pLineRequest = pLineRequest;
  return ResultCode::OK;
}

ResultCode
GpioOutputLinesSource::requestLines(const char* context, GpioOutputLinesRequestVector& requests)
{
  for (auto& request : requests) {
    ResultCode rc = requestLines(context, request);
    if (rc != ResultCode::OK) return rc;
  }
  return ResultCode::OK;
}