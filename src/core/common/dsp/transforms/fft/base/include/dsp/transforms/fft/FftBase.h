#pragma once

#include <samples/SampleTypes.h>
#include <dsp/window/Window.h>

class FftBase
{
public:

  explicit FftBase(WindowType window = WindowType::NONE);
  virtual ~FftBase() = default;

  virtual uint32_t transform(
    const ComplexSamplesFft& input,
    ComplexSamplesFft& output,
    bool forward,
    bool normalise
  ) = 0;

  virtual uint32_t transform(
    const ComplexSamplesBuffer& input,
    ComplexSamplesBuffer& output,
    uint32_t inputLength,
    bool forward,
    bool normalise
  ) = 0;

protected:
  WindowFunction m_window;
};