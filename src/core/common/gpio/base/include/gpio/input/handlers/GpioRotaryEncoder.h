#pragma once
#include "gpio/input/GpioLineEvent.h"
#include "gpio/input/GpioLineReader.h"
#include "gpio/input/handlers/GpioLineTransitionHandler.h"

class GpioRotaryEncoder : public GpioLineTransitionHandler
{
public:
  GpioRotaryEncoder(GpioLineMask lineA, GpioLineMask lineB, GpioLineReader& reader);
  bool handleLineTransition(GpioLineMask mask, Timestamp timestamp, GpioLineEvent* pEvent) override;

  GpioRotaryEncoder(const GpioRotaryEncoder& other) = default;
  GpioRotaryEncoder(GpioRotaryEncoder&& other) noexcept = default;
  GpioRotaryEncoder& operator=(const GpioRotaryEncoder& other) ;
  GpioRotaryEncoder& operator=(GpioRotaryEncoder&& other) noexcept;

protected:
  [[nodiscard]] uint8_t getCurrentState() const;

private:
  GpioLineMask m_lineA;
  GpioLineMask m_lineB;
  uint8_t m_previousState;
  GpioLineReader& m_reader;
};
