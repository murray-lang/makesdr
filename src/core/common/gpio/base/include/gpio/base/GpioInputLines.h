#pragma once
#include "GpioLines.h"
#include "config/struct/GpioInputLinesConfig.h"

class GpioInputLines : public GpioLines
{
public:
  GpioInputLines()
    : GpioLines(Direction::INPUT)
    , m_debounce(false)
    , m_activeHigh(true)
    , m_rotaryEncoder(false)
  {}
  GpioInputLines(GpioInputLines&&)  noexcept = default;

  ~GpioInputLines()override = default;

  GpioInputLines& operator=(GpioInputLines&&)  noexcept = default;


  [[nodiscard]] bool isDebounce() const { return m_debounce; }
  [[nodiscard]] bool isActiveHigh() const { return m_activeHigh; }
  [[nodiscard]] bool isRotaryEncoder() const { return m_rotaryEncoder; }

  ResultCode configureLines(const Config::GpioInputLines::Fields& config)
  {
    ResultCode rc = GpioLines::configureLines(config);
    if (rc != ResultCode::OK) return rc;

    m_debounce = config.debounce;
    m_activeHigh = config.activeHigh;
    m_rotaryEncoder = config.rotaryEncoder;

    return ResultCode::OK;
  }
protected:
  bool m_debounce;
  bool m_activeHigh;
  bool m_rotaryEncoder;
};

#ifdef USE_ETL
#include <etl/vector.h>
using GpioInputLinesVector = etl::vector<GpioInputLines, MAX_DIGITAL_INPUT_HANDLERS>;
#else
#include <vector>
using GpioInputLinesVector = std::vector<GpioInputLines>;
#endif
