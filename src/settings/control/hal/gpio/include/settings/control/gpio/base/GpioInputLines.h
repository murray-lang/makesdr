#pragma once
#include "GpioLines.h"
#include "config/struct/GpioInputLinesConfig.h"

class GpioInputLines : public GpioLines
{
public:
  GpioInputLines() : GpioLines(Direction::INPUT), m_debounce(false), m_activeHigh(true) {}
  GpioInputLines(GpioInputLines&&)  noexcept = default;

  ~GpioInputLines()override = default;

  GpioInputLines& operator=(GpioInputLines&&)  noexcept = default;


  [[nodiscard]] bool getDebounce() const { return m_debounce; }
  [[nodiscard]] bool getActiveHigh() const { return m_activeHigh; }

  ResultCode configureLines(const Config::GpioInputLines::Fields& config)
  {
    ResultCode rc = GpioLines::configureLines(config);
    if (rc != ResultCode::OK) return rc;

    m_debounce = config.debounce;
    m_activeHigh = config.activeHigh;

    return ResultCode::OK;
  }
protected:
  bool m_debounce;
  bool m_activeHigh;
};

#ifdef USE_ETL
#include <etl/vector.h>
using GpioInputLinesVector = etl::vector<GpioInputLines, MAX_GPIO_LINES_PER_DEVICE>;
#else
#include <vector>
using GpioInputLinesVector = std::vector<GpioInputLines>;
#endif
