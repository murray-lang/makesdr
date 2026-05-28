#pragma once

#include "DigitalInput.h"
#include "DigitalInputLinesRequest.h"
#include "config/RotaryEncoderConfig.h"

class RotaryEncoder : public DigitalInput
{
public:
  RotaryEncoder();

  RotaryEncoder(RotaryEncoder&& rhs) = default;
  RotaryEncoder& operator=(RotaryEncoder&& rhs) = default;

  virtual ResultCode configure(const Config::RotaryEncoder::Fields& config);

  bool handleLineChange(DigitalInputLinesRequest::LineStateVector& changedLines) override;

protected:
  void notifyMovement(int movement);
  static int calculateMovement(DigitalInputLinesRequest::LineState& a, DigitalInputLinesRequest::LineState& b);
};
