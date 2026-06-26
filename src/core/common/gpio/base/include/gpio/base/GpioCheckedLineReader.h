#pragma once
#include "GpioLineMask.h"
#include "GpioLineReaderDelegate.h"


class GpioCheckedLineReader
{
  friend class GpioInputLinesRequest;
public:
  GpioCheckedLineReader()
    : m_lineReaderDelegate(defaultReader)
    , m_permittedLines(0)
  {}

  ResultCode readLine(GpioLineMask lineMask, GpioLineValue* lineValue) const
  {
    GpioLineMask permittedLines = lineMask & m_permittedLines;
    if (permittedLines == 0) {
      return ResultCode::ERR_GPIO_INPUT_LINE_NOT_PERMITTED;
    }
    return m_lineReaderDelegate(permittedLines, lineValue);
  }

  void apply(GpioLineMask permittedLines, GpioLineReaderDelegate lineReaderDelegate)
  {
    m_permittedLines = permittedLines;
    m_lineReaderDelegate = lineReaderDelegate;
  }

  void setPermittedLines(GpioLineMask permittedLines) { m_permittedLines = permittedLines; }
  void setLineReaderDelegate(GpioLineReaderDelegate lineReaderDelegate) { m_lineReaderDelegate = lineReaderDelegate; }
  GpioLineReaderDelegate& getLineReaderDelegate() { return m_lineReaderDelegate; }

private:
  static ResultCode defaultReader(GpioLineMask, GpioLineValue*)
  {
    return ResultCode::ERR_GPIO_INPUT_LINE_NOT_PERMITTED;
  }

  GpioLineReaderDelegate m_lineReaderDelegate;
  GpioLineMask m_permittedLines;
};
