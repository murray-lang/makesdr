#pragma once
#include "gpio/input/GpioLineEvent.h"
#include "GpioLineTransitionHandler.h"
#include "gpio/input/GpioLineReader.h"
#include <time/Timestamp.h>

class GpioLineDebouncer : public GpioLineTransitionHandler
{
public:
  GpioLineDebouncer(
    GpioLineMask lines,
    bool activeHigh,
    GpioLineReader& reader,
    Timestamp debouncePeriod = 0
    );

  GpioLineDebouncer(const GpioLineDebouncer& other) = default;
  GpioLineDebouncer(GpioLineDebouncer&& other) noexcept = default;
  GpioLineDebouncer& operator=(const GpioLineDebouncer& other) ;
  GpioLineDebouncer& operator=(GpioLineDebouncer&& other) noexcept;

  bool handleLineTransition(GpioLineMask mask, Timestamp timestamp, GpioLineEvent* pEvent) override;

  [[nodiscard]] bool isDebounced(uint32_t now) const
  {
    return m_debounceStartTicks > 0 && m_debounceStartTicks + m_debouncePeriod <= now;
  }

  [[nodiscard]] bool isDebouncedNow() const
  {
    return isDebounced(now());
  }

  void resetDebouncing() { m_debounceStartTicks = 0; }

  void getEvent(GpioLineEvent* event);

protected:
  [[nodiscard]] GpioLineValue getCurrentValue() const;

private:
  GpioLineMask m_lines;
  bool m_activeHigh;
  Timestamp m_debouncePeriod;
  Timestamp m_debounceStartTicks;
  GpioLineReader& m_reader;
};
