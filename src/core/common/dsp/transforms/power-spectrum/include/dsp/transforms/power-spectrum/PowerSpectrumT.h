#pragma once
#include <algorithm>
#include <cmath>
#include <limits>
#include <dsp/transforms/fft/Fft.h>

template<typename T, uint32_t FFT_SIZE>
class PowerSpectrumT
{
public:
  PowerSpectrumT() : PowerSpectrumT(-120.0f, 0.0f) {}
  PowerSpectrumT(float floor, float ceiling)
    : m_floor(floor)
    , m_ceiling(ceiling)
    , m_fft(WindowType::HANNING)
  {
    m_spectrum.resize(FFT_SIZE);
    m_power.resize(FFT_SIZE);
    reset();
  }

  void setRange(float floor, float ceiling)
  {
    m_floor = floor;
    m_ceiling = ceiling;
  }

  [[nodiscard]] float floor() const { return m_floor; }
  [[nodiscard]] float ceiling() const { return m_ceiling; }

  // Single-shot: power spectrum of one buffer, averaged across its FFT_SIZE chunks
  uint32_t transform(const sdrcomplex* input, uint32_t inputLength, T* output)
  {
    reset();
    if (accumulate(input, inputLength) == 0) {
      return 0;
    }
    render(output);
    return inputLength;
  }

  // Discard any accumulated power
  void reset()
  {
    m_power.assign(FFT_SIZE, 0.0f);
    m_chunks = 0;
  }

  [[nodiscard]] uint32_t chunks() const { return m_chunks; }

  // Add the power of each FFT_SIZE chunk of input to the running sum. Can be called
  // repeatedly (e.g. across several buffers) before render() to average over more data.
  uint32_t accumulate(const sdrcomplex* input, uint32_t inputLength)
  {
    const uint32_t repeats = inputLength / FFT_SIZE;
    for (uint32_t chunk = 0; chunk < repeats; ++chunk) {
      m_fft.transform(input + (chunk * FFT_SIZE), m_spectrum.data(), FFT_SIZE, true, true);
      for (uint32_t i = 0; i < FFT_SIZE; ++i) {
        m_power[i] += std::norm(m_spectrum[i]);   // |X|^2
      }
    }
    m_chunks += repeats;
    return repeats * FFT_SIZE;
  }

  // Quantise the average of everything accumulated since reset() into output, then reset.
  // output must hold FFT_SIZE values. Returns false (output untouched) if nothing was accumulated.
  bool render(T* output)
  {
    // Bin value b maps linearly to dBFS: dB = m_floor + b * (m_ceiling - m_floor) / max(OutT)
    // Hann coherent gain. With 1/N normalisation a full-scale tone gives |X| = 0.5
    constexpr float WINDOW_COHERENT_GAIN = 0.5f;

    if (m_chunks == 0) {
      return false;
    }
    const auto minValue = static_cast<float>(std::numeric_limits<T>::min());
    const auto maxValue = static_cast<float>(std::numeric_limits<T>::max());

    // Average across chunks and undo the window gain so a full-scale tone reads 0 dBFS
    const float scale = 1.0f / (static_cast<float>(m_chunks) * WINDOW_COHERENT_GAIN * WINDOW_COHERENT_GAIN);
    const float valuePerDb = maxValue / (m_ceiling - m_floor);
    constexpr uint32_t half = FFT_SIZE / 2;

    for (uint32_t i = 0; i < FFT_SIZE; ++i) {
      // fftshift: negative frequencies on the left, DC in the middle
      const float power = m_power[(i + half) % FFT_SIZE] * scale;
      const float db = 10.0f * std::log10(power + 1e-20f);
      const float level = std::clamp((db - m_floor) * valuePerDb, minValue, maxValue);
      output[i] = static_cast<T>(std::lround(level));
    }
    reset();
    return true;
  }

  // Inverse of transform()'s quantisation: bin value b -> dB = m_floor + b * (m_ceiling - m_floor) / max(T)
  uint32_t rescale(const T* input, uint32_t inputLength, sdrreal* output) const
  {
    const auto maxValue = static_cast<float>(std::numeric_limits<T>::max());
    const float dbPerValue = (m_ceiling - m_floor) / maxValue;

    for (uint32_t i = 0; i < inputLength; ++i) {
      output[i] = (m_floor + static_cast<float>(input[i]) * dbPerValue);
    }
    return inputLength;
  }

protected:
  float m_floor;
  float m_ceiling;
  Fft<FFT_SIZE> m_fft;
  etl::vector<sdrcomplex, FFT_SIZE> m_spectrum;
  etl::vector<sdrreal, FFT_SIZE> m_power;
  uint32_t m_chunks = 0;   // FFT_SIZE chunks summed into m_power since reset()
};