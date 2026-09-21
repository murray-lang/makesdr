#ifndef STM32H745I_FFT_H
#define STM32H745I_FFT_H

#include <samples/SampleTypes.h>
#include <stm32h745i/math/arm_math.h>
#include <stm32h745i/math/arm_const_structs.h>

#include <cstring>

template<typename BufferType>
class Fft
{
public:
  explicit Fft(WindowType window = WindowType::NONE);

  uint32_t transform(
    const BufferType& input,
    BufferType& output,
    uint32_t inputLength,
    bool forward,
    bool normalise
    )
  {
    // STM32 FFT is done in-place, so copy the input to the output now and perform
    // the FFT on the output buffer.
    if (forward ) {
      for (uint32_t i = 0; i < inputLength; ++i) {
        output.at(i) = input.at(i) * m_window(i, inputLength);
      }
    } else if (output.data() != input.data()){
      memcpy(output.data(), input.data(), inputLength * sizeof(sdrcomplex) );
    }

    auto* interleavedBuffer = reinterpret_cast<sdrreal*>(output.data());

    // Perform FFT (0 = forward, 1 = inverse)
    uint32_t ifftFlag = forward ? 0 : 1;

    const arm_cfft_instance_f32* fftInstance =
      inputLength == 1024 ? &arm_cfft_sR_f32_len1024 :
      inputLength == 2048 ? &arm_cfft_sR_f32_len2048 :
      inputLength == 4096 ? &arm_cfft_sR_f32_len4096 :
      nullptr;

    arm_cfft_f32(fftInstance, interleavedBuffer, ifftFlag, 1);

    if (normalise) {
      for (uint32_t i = 0; i < inputLength; ++i) {
        output[i] /= static_cast<sdrreal>(inputLength);
      }
    }

    return inputLength;
  }

};

#endif // STM32H745I_FFT_H
