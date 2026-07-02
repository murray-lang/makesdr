#pragma once
#include <gpio/base/GpioLines.h>
#include "GpioConsumerString.h"
#include <gpiod.h>

class GpioLinesSourceBase
{
public:
  GpioLinesSourceBase()
    : m_pChip(nullptr)
    , m_pLineRequest(nullptr)
  {
  }

  virtual ~GpioLinesSourceBase() { release(); }

  void initialise(gpiod_chip* pChip, const char* consumer = "")
  {
    m_pChip = pChip;
    m_consumer = consumer;
  }

  void release()
  {
    if (m_pLineRequest) {
      gpiod_line_request_release(m_pLineRequest);
      m_pLineRequest = nullptr;
    }
  }

  static int getLineNumbers(GpioLineMask lineMask, GpioLineNumberVector& lineNumbers)
  {
    int count = 0;
    for (int i = 0; i < MAX_GPIO_LINES; i++) {
      GpioLineMask line = lineMask & (1 << i);
      if (line == 0) continue;
      uint32_t lineNo = __builtin_ctz(line);
      lineNumbers.push_back(lineNo);
      count++;
    }
    return count;
  }

protected:
  gpiod_chip* m_pChip;
  GpioConsumerString m_consumer;
  gpiod_line_request * m_pLineRequest;
};