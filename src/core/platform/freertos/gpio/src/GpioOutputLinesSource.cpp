#include "gpio/service/GpioOutputLinesSource.h"

ResultCode
GpioOutputLinesSource::initialise(void* gpioHandle, const char * context)
{
  return ResultCode::OK;
}

void
GpioOutputLinesSource::release()
{

}

ResultCode
GpioOutputLinesSource::requestLines(const char* context, GpioOutputLinesRequest* request)
{
  GpioLineMask requestedLineMask = request->config.getLines();
  for (int i = 0; i < MAX_GPIO_LINES; i++) {
    GpioLineMask lineMask = requestedLineMask & (1 << i);

    if (lineMask == 0) continue;

    if (!isGpioOutputLineAvailable(lineMask)) {
      return ResultCode::ERR_GPIO_INPUT_LINE_NOT_AVAILABLE;
    }
  }
  request->lineWriter = gpioWriteLine;
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