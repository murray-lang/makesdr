#pragma once
#include "../base/GpioLineEvent.h"
#include "GpioLineTransitionHandler.h"
#include "gpio/base/GpioLineReaderDelegate.h"

class GpioRotaryEncoder : public GpioLineTransitionHandler
{
public:
  GpioRotaryEncoder(GpioLineMask lineA, GpioLineMask lineB, GpioLineReaderDelegate& reader);
  bool handleLineTransition(GpioLineMask mask, GpioTimestamp timestamp, GpioLineEvent* pEvent) override;

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
  GpioLineReaderDelegate& m_reader;
};
