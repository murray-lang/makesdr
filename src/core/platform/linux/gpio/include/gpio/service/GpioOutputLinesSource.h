#ifndef LINUX_GPIOOUTPUTLINESSOURCE_H_
#define LINUX_GPIOOUTPUTLINESSOURCE_H_

#include "gpio/service/GpioLinesSourceBase.h"
#include "gpio/output/GpioOutputLinesRequest.h"
#include "ResultCode.h"


class GpioOutputLinesSource : public GpioLinesSourceBase
{
public:
  GpioOutputLinesSource();

  ResultCode requestLines(const char* context, GpioOutputLinesRequest* request);
  ResultCode requestLines(const char* context, GpioOutputLinesRequestVector& requests);

  ResultCode writeLine(GpioLineMask mask, const GpioLineValue& value);
protected:

  GpioLineWriter m_lineWriter;

};
#endif // LINUX_GPIOOUTPUTLINESSOURCE_H_
