#include "gpio/input/GpioLineReader.h"
#include "gpio/input/handlers/GpioRotaryEncoder.h"

// #include <stm32h745i/app/support/safe_printf.h>

GpioRotaryEncoder::GpioRotaryEncoder(GpioLineMask lineA, GpioLineMask lineB, GpioLineReader& reader)
  : m_lineA(lineA)
  , m_lineB(lineB)
  , m_previousState(0)
  , m_reader(reader)
{
}

GpioRotaryEncoder&
GpioRotaryEncoder::operator=(const GpioRotaryEncoder& other)
{
  if (this != &other) {
    m_lineA = other.m_lineA;
    m_lineB = other.m_lineB;
    m_previousState = 0;
    m_reader = other.m_reader;
  }
  return *this;
}

GpioRotaryEncoder&
GpioRotaryEncoder::operator=(GpioRotaryEncoder&& other) noexcept
{
  m_lineA = other.m_lineA;
  m_lineB = other.m_lineB;
  m_previousState = 0;
  m_reader = other.m_reader;
  return *this;
}

bool
GpioRotaryEncoder::handleLineTransition(GpioLineMask mask, Timestamp timestamp, GpioLineEvent* pEvent)
{
  if ( (mask == m_lineA)  || (mask == m_lineB))  {
    pEvent->mask = mask;
    pEvent->timestamp = timestamp;
    uint8_t currentState = getCurrentState();

    // If no change occurred, ignore (helps filter out minor noise)
    if (currentState != m_previousState) {
      /* Standard Quadrature State Machine Matrix
         Compares previous 2-bit state to the new 2-bit state */
      switch ((m_previousState << 2) | currentState)
      {
        // Clockwise transitions
      case 0b0001: case 0b0111: case 0b1110: case 0b1000:
        pEvent->value = 1;
        break;

        // Counter-Clockwise transitions
      case 0b0010: case 0b1011: case 0b1101: case 0b0100:
        pEvent->value = -1;
        break;

      default:
        // Invalid transition step (can occur during fast spinning or bounce)
        pEvent->value = 0;
        break;
      }

      m_previousState = currentState;

      // Only report event when reaching stable detent position (both lines low)
      if (currentState == 0b00 && pEvent->value != 0) {
        return true;
      }
    }
  }
  return false;
}

uint8_t
GpioRotaryEncoder::getCurrentState() const
{
  GpioLineValue chA;
  GpioLineValue chB;
  m_reader(m_lineA, &chA);
  m_reader(m_lineB, &chB);

  return (chA << 1) | chB;
}
