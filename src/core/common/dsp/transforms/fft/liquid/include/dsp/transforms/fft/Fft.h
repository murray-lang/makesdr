#ifndef LIQUID_FFT_H
#define LIQUID_FFT_H

#include <samples/SampleTypes.h>
#include <dsp/window/Window.h>
#include <liquid/liquid.h>
#include <cstring>

template<typename BufferType>
class Fft
{
public:
  explicit Fft(WindowType window = WindowType::NONE)
    : m_forwardPlan(nullptr)
    , m_inversePlan(nullptr)
  {
    m_window =  window == WindowType::HAMMING ? window_hamming
            : window == WindowType::HANNING ? window_hanning
            : window == WindowType::BLACKMAN ? window_blackman
            : window_none;

    // Create FFT plan with the internal buffers
    m_forwardPlan = fft_create_plan(FFT_SIZE,
                            reinterpret_cast<liquid_float_complex*>(m_inputBuffer.data()),
                            reinterpret_cast<liquid_float_complex*>(m_outputBuffer.data()),
                            LIQUID_FFT_FORWARD,
                            0);
    m_inversePlan = fft_create_plan(FFT_SIZE,
                              reinterpret_cast<liquid_float_complex*>(m_inputBuffer.data()),
                              reinterpret_cast<liquid_float_complex*>(m_outputBuffer.data()),
                              LIQUID_FFT_BACKWARD,
                              0);
  }
  ~Fft()
  {
    if (m_forwardPlan != nullptr) {
      fft_destroy_plan(m_forwardPlan);
    }
    if (m_inversePlan != nullptr) {
      fft_destroy_plan(m_inversePlan);
    }
  }

  uint32_t transform(
    const BufferType& input,
    BufferType& output,
    uint32_t inputLength,
    bool forward,
    bool normalise
  )
  {

    // Apply window to input for forward transform
    if (forward) {
      for (uint32_t i = 0; i < inputLength; ++i) {
        m_inputBuffer.at(i) = input.at(i) * m_window(i, inputLength);
      }
    }
    else {
      std::memcpy(m_inputBuffer.data(), input.data(), inputLength * sizeof(sdrcomplex));
    }

    // Execute the transform
    fft_execute(forward ? m_forwardPlan : m_inversePlan);

    // Resize output and copy from internal buffer
    // output.resize(numSamples);
    std::memcpy(output.data(), m_outputBuffer.data(), inputLength * sizeof(sdrcomplex));

    if (normalise) {
      for (uint32_t i = 0; i < inputLength; ++i) {
        output[i] /= static_cast<sdrreal>(inputLength);
      }
    }

    return inputLength;
  }


protected:
  WindowFunction m_window;
  fftplan m_forwardPlan;
  fftplan m_inversePlan;
  BufferType m_inputBuffer;
  BufferType m_outputBuffer;
};

#endif
