#pragma once
#include "gpio/base/GpioLineEvent.h"
#include "GpioLineTransitionHandler.h"
#include "gpio/base/GpioLineReaderDelegate.h"
#include "gpio/base/GpioNowGetter.h"

class GpioLineDebouncer : public GpioLineTransitionHandler
{
public:
  GpioLineDebouncer(
    GpioLineMask lines,
    bool activeHigh,
    GpioLineReaderDelegate& reader,
    GpioNowGetter* nowGetter,
    GpioTimestamp debouncePeriod = 0
    );

  GpioLineDebouncer(const GpioLineDebouncer& other) = default;
  GpioLineDebouncer(GpioLineDebouncer&& other) noexcept = default;
  GpioLineDebouncer& operator=(const GpioLineDebouncer& other) ;
  GpioLineDebouncer& operator=(GpioLineDebouncer&& other) noexcept;

  bool handleLineTransition(GpioLineMask mask, GpioTimestamp timestamp, GpioLineEvent* pEvent) override;

  [[nodiscard]] bool isDebounced(uint32_t now) const
  {
    return m_debounceStartTicks > 0 && m_debounceStartTicks + m_debouncePeriod <= now;
  }

  [[nodiscard]] bool isDebouncedNow() const
  {
    return isDebounced(m_nowGetter->now());
  }

  void resetDebouncing() { m_debounceStartTicks = 0; }

  void getEvent(GpioLineEvent* event);

protected:
  [[nodiscard]] GpioLineValue getCurrentValue() const;

private:
  GpioLineMask m_lines;
  bool m_activeHigh;
  GpioTimestamp m_debouncePeriod;
  GpioTimestamp m_debounceStartTicks;
  GpioLineReaderDelegate& m_reader;
  GpioNowGetter* m_nowGetter;
};
