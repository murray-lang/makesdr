#include "gpio/handlers/GpioLineDebouncer.h"

GpioLineDebouncer::GpioLineDebouncer(
    GpioLineMask lines,
    bool activeHigh,
    GpioLineReaderDelegate& reader,
    GpioNowGetter* nowGetter,
    GpioTimestamp debouncePeriod
    )
  : m_lines(lines)
  , m_activeHigh(activeHigh)
  , m_debouncePeriod(debouncePeriod)
  , m_debounceStartTicks(0)
  , m_reader(reader)
  , m_nowGetter(nowGetter)
{
}

GpioLineDebouncer&
GpioLineDebouncer::operator=(const GpioLineDebouncer& other)
{
  if (this != &other) {
    m_lines = other.m_lines;
    m_activeHigh = other.m_activeHigh;
    m_debouncePeriod = other.m_debouncePeriod;
    m_debounceStartTicks = other.m_debounceStartTicks;
    m_reader = other.m_reader;
    m_nowGetter = other.m_nowGetter;
  }
  return *this;
}

GpioLineDebouncer&
GpioLineDebouncer::operator=(GpioLineDebouncer&& other) noexcept
{
  m_lines = other.m_lines;
  m_activeHigh = other.m_activeHigh;
  m_debouncePeriod = other.m_debouncePeriod;
  m_debounceStartTicks = other.m_debounceStartTicks;
  m_reader = other.m_reader;
  m_nowGetter = other.m_nowGetter;
  return *this;
}

bool
GpioLineDebouncer::handleLineTransition(GpioLineMask mask, GpioTimestamp timestamp, GpioLineEvent* pEvent)
{
  bool result = false;
  if (mask & m_lines) {
    pEvent->changed = true;
    pEvent->timestamp = timestamp;
    pEvent->value = getCurrentValue();

    if (m_debounceStartTicks == 0) {
      m_debounceStartTicks = timestamp;
    } else if (isDebounced(timestamp)) {
      result = true;
    }
  }
  return result;
}

void
GpioLineDebouncer::getEvent(GpioLineEvent* event)
{
  event->mask = m_lines;
  event->changed = true;
  event->timestamp = m_nowGetter->now();
  event->value = getCurrentValue();
}

GpioLineValue
GpioLineDebouncer::getCurrentValue() const
{
  GpioLineValue rawValue;
  m_reader(m_lines, &rawValue);
  return rawValue ? m_activeHigh ? 1 : 0 : 0;
}
