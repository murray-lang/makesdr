#ifndef STM32H745I_FFT_H
#define STM32H745I_FFT_H

#include <samples/SampleTypes.h>
#include <stm32h745i/math/arm_math.h>
#include <stm32h745i/math/arm_const_structs.h>
#include <dsp/window/Window.h>

#include <cstring>


template<uint32_t MAX_SIZE>
class Fft
{
public:
  explicit Fft(WindowType window = WindowType::NONE);

  uint32_t transform(
    const sdrcomplex* input,
    sdrcomplex* output,
    uint32_t inputLength,
    bool forward,
    bool normalise
    );

protected:
  WindowType m_windowType;
  WindowFunction m_window;

};

#endif // STM32H745I_FFT_H
