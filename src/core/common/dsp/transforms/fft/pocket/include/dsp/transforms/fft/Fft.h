#ifndef POCKET_FFT_H
#define POCKET_FFT_H

#include <samples/SampleTypes.h>
#include <dsp/window/Window.h>
#include "pocketfft_hdronly.h"

template<uint32_t MAX_SIZE> // Not required for PocketFFT
class Fft
{
public:

  Fft(WindowType window = WindowType::NONE)
      : m_windowType(window)
      , m_pocketfft_stride{sizeof(sdrcomplex)}
  , m_pocketfft_axes{0}
  {
    m_window =  window == WindowType::HAMMING ? window_hamming
            : window == WindowType::HANNING ? window_hanning
            : window == WindowType::BLACKMAN ? window_blackman
            : window_none;
    // m_windowBuffer.resize(m_windowBuffer.max_size());
  }

  uint32_t transform(
    const sdrcomplex* input,
    sdrcomplex* output,
    uint32_t inputLength,
    bool forward,
    bool normalise)
  {
    const sdrcomplex* inBuffer = input;

    if (forward && m_windowType != WindowType::NONE) {
      for (uint32_t i = 0; i < inputLength; ++i) {
        output[i] = input[i] * m_window(i, inputLength);
      }
      inBuffer = output;
    }
    pocketfft::shape_t pocketfft_shape{inputLength};

    pocketfft::c2c(
        pocketfft_shape,
        m_pocketfft_stride,
        m_pocketfft_stride,
        m_pocketfft_axes,
        forward,
        inBuffer,
        output,
        normalise ? (static_cast<sdrreal>(1.0)/static_cast<sdrreal>(inputLength)) : static_cast<sdrreal>(1.0)
        );

    return inputLength;
  }

protected:
  WindowType m_windowType;
  WindowFunction m_window;
  pocketfft::stride_t m_pocketfft_stride;
  pocketfft::shape_t m_pocketfft_axes;

  // BufferType m_windowBuffer;
};

#endif // POCKET_FFT_H
