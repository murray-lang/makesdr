#pragma once

#include "ColorMap.h"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include <settings/model/radio/iq/FftMessage.h>
#include <dsp/transforms/power-spectrum/PowerSpectrumT.h>


// One line of a spectrum stream: power per FFT bin (dB) across a span centred
// on a frequency. If `shuffled` is true the bins are in raw FFT order (DC at
// index 0, negative frequencies in the upper half) and are re-ordered here.
template<typename binType>
struct SpectrumFrame
{
  const binType* bins = nullptr;
  size_t binCount = 0;
  int64_t centreFrequency = 0;
  uint32_t span = 0;          // Hz, normally the IQ sample rate
  bool shuffled = true;
};

// Platform-neutral waterfall. Consumes a stream of SpectrumFrames and renders
// each as one row of pixels, using bin power as the index into a ColorMap.
//
// The pixel storage is owned by the caller (QImage bits, an LVGL canvas
// buffer, a static array on an MCU) so this class never allocates. Newest row
// is at the top.
//
// Two scroll modes:
//  - Ring:  O(width) per frame. Rows are written into a ring buffer and the
//           platform blits up to two segments (see segments()).
//  - Shift: O(width*height) per frame. The buffer is always in display order
//           so it can be shown directly, e.g. as an LVGL canvas.
//
// Retuning with an unchanged span shifts the history sideways so old signals
// stay aligned with their frequency; a span change clears the history.
template<typename Format>
class Waterfall
{
public:
  using Pixel = typename Format::Pixel;

  enum class ScrollMode { Ring, Shift };

  // A run of buffer rows to draw at a display row offset
  struct Segment
  {
    uint16_t sourceRow;
    uint16_t displayRow;
    uint16_t rows;
  };

  Waterfall() = default;

  void attach(Pixel* pixels, uint16_t width, uint16_t height, size_t strideBytes, ScrollMode mode = ScrollMode::Ring)
  {
    m_pixels = reinterpret_cast<uint8_t*>(pixels);
    m_width = width;
    m_height = height;
    m_strideBytes = strideBytes;
    m_mode = mode;
    clear();
  }

  void detach() { m_pixels = nullptr; m_width = 0; m_height = 0; }

  [[nodiscard]] bool isAttached() const { return m_pixels != nullptr && m_width > 0 && m_height > 0; }
  [[nodiscard]] uint16_t width() const { return m_width; }
  [[nodiscard]] uint16_t height() const { return m_height; }

  ColorMap<Format>& colorMap() { return m_colorMap; }

  // Power mapped to the bottom and top of the color map
  void setDbRange(float minDb, float maxDb)
  {
    m_minDb = minDb;
    m_maxDb = maxDb > minDb ? maxDb : minDb + 1.0f;
  }
  [[nodiscard]] float minDb() const { return m_minDb; }
  [[nodiscard]] float maxDb() const { return m_maxDb; }

  void clear()
  {
    m_head = 0;
    m_hasHistory = false;
    if (!isAttached()) {
      return;
    }
    Pixel background = m_colorMap[0];
    for (uint16_t row = 0; row < m_height; row++) {
      fill(rowPointer(row), 0, m_width, background);
    }
  }

  // struct SpectrumFrame
  // {
  //   const binType* bins = nullptr;
  //   size_t binCount = 0;
  //   int64_t centreFrequency = 0;
  //   uint32_t span = 0;          // Hz, normally the IQ sample rate
  //   bool shuffled = true;
  // };

  void addSpectrum(const FftMessage* fftMsg, int64_t centreFrequency)
  {
    float floor = fftMsg->floor();
    float ceiling = fftMsg->ceiling();

    if (m_minDb != floor || m_maxDb != ceiling) {
      setDbRange(floor, ceiling);
    }
    uint32_t numBins = fftMsg->bins().size;
    PowerSpectrumT<uint8_t, MONITOR_FFT_SIZE > powerSpectrum(floor, ceiling);
    RealMonitorFft scaled;
    powerSpectrum.rescale(fftMsg->bins().bytes, numBins, scaled.data());

    SpectrumFrame<float> frame {
      .bins = scaled.data(),
      .binCount = numBins,
      .centreFrequency = centreFrequency,
      .span = fftMsg->sampleRate(),
      .shuffled = false
    };
    addSpectrum<float>(frame);
  }

  // Append one frame as the newest row
  template<typename binType>
  void addSpectrum(const SpectrumFrame<binType>& frame)
  {
    if (!isAttached() || frame.bins == nullptr || frame.binCount == 0 || frame.span == 0) {
      return;
    }
    double startFrequency = static_cast<double>(frame.centreFrequency) - static_cast<double>(frame.span) / 2.0;
    alignHistory(startFrequency, frame.span);

    Pixel* row = rowPointer(advance());
    renderRow<binType>(frame, row);
  }

  // Up to two segments that together cover the display, newest first.
  // Returns the number of segments written to `out`.
  size_t segments(Segment out[2]) const
  {
    if (!isAttached()) {
      return 0;
    }
    out[0] = { m_head, 0, static_cast<uint16_t>(m_height - m_head) };
    if (m_head == 0) {
      return 1;
    }
    out[1] = { 0, static_cast<uint16_t>(m_height - m_head), m_head };
    return 2;
  }

  // Pixels of a buffer row (as numbered by Segment::sourceRow)
  [[nodiscard]] const Pixel* bufferRow(uint16_t bufferRow) const
  {
    return reinterpret_cast<const Pixel*>(m_pixels + static_cast<size_t>(bufferRow) * m_strideBytes);
  }

  // Frequency under pixel column x (for click-to-tune), relative to the newest row
  [[nodiscard]] double frequencyAt(double x) const
  {
    if (!m_hasHistory || m_width == 0) {
      return 0.0;
    }
    return m_startFrequency + x * static_cast<double>(m_span) / static_cast<double>(m_width);
  }

private:
  Pixel* rowPointer(uint16_t bufferRow)
  {
    return reinterpret_cast<Pixel*>(m_pixels + static_cast<size_t>(bufferRow) * m_strideBytes);
  }

  static void fill(Pixel* row, uint16_t from, uint16_t to, Pixel value)
  {
    for (uint16_t x = from; x < to; x++) {
      row[x] = value;
    }
  }

  // Make room for a new row and return the buffer row to write it into
  uint16_t advance()
  {
    if (m_mode == ScrollMode::Shift) {
      // Rows may be padded, so move whole strides; row 0 is then overwritten
      std::memmove(m_pixels + m_strideBytes, m_pixels, (static_cast<size_t>(m_height) - 1) * m_strideBytes);
      return 0;
    }
    m_head = static_cast<uint16_t>(m_head == 0 ? m_height - 1 : m_head - 1);
    return m_head;
  }

  // Keep existing rows aligned in frequency with the incoming frame
  void alignHistory(double startFrequency, uint32_t span)
  {
    if (!m_hasHistory || span != m_span) {
      if (m_hasHistory) {
        clear();
      }
      m_hasHistory = true;
      m_span = span;
      m_startFrequency = startFrequency;
      return;
    }

    double hzPerPixel = static_cast<double>(span) / static_cast<double>(m_width);
    auto shift = static_cast<long>(std::lround((startFrequency - m_startFrequency) / hzPerPixel));
    if (shift == 0) {
      return;
    }
    if (shift >= m_width || shift <= -static_cast<long>(m_width)) {
      clear();
      m_hasHistory = true;
      m_span = span;
      m_startFrequency = startFrequency;
      return;
    }
    shiftRows(static_cast<int>(shift));
    // Anchor to whole pixels so repeated small retunes don't accumulate drift
    m_startFrequency += static_cast<double>(shift) * hzPerPixel;
  }

  // Positive shift moves content left (tuned up in frequency)
  void shiftRows(int shift)
  {
    Pixel background = m_colorMap[0];
    auto magnitude = static_cast<uint16_t>(shift < 0 ? -shift : shift);
    size_t keep = static_cast<size_t>(m_width - magnitude);
    for (uint16_t r = 0; r < m_height; r++) {
      Pixel* row = rowPointer(r);
      if (shift > 0) {
        std::memmove(row, row + magnitude, keep * sizeof(Pixel));
        fill(row, static_cast<uint16_t>(keep), m_width, background);
      } else {
        std::memmove(row + magnitude, row, keep * sizeof(Pixel));
        fill(row, 0, magnitude, background);
      }
    }
  }

  template<typename binType>
  void renderRow(const SpectrumFrame<binType>& frame, Pixel* row) const
  {
    const size_t bins = frame.binCount;
    const size_t half = frame.shuffled ? bins / 2 : 0;
    // const float scale = static_cast<float>(ColorMap<Format>::LEVELS - 1) / (m_maxDb - m_minDb);
    const float scale = static_cast<float>(ColorMap<Format>::LEVELS - 1) / (255.0);

    // Sub-pixel offset of this frame relative to the aligned history
    double startFrequency = static_cast<double>(frame.centreFrequency) - static_cast<double>(frame.span) / 2.0;
    double binsPerHz = static_cast<double>(bins) / static_cast<double>(frame.span);
    double binOffset = (m_startFrequency - startFrequency) * binsPerHz;
    double binsPerPixel = static_cast<double>(bins) / static_cast<double>(m_width);

    for (uint16_t x = 0; x < m_width; x++) {
      // Each pixel covers [first, last) bins; take the peak so narrow
      // signals survive when there are more bins than pixels
      auto first = static_cast<long>(std::floor(binOffset + x * binsPerPixel));
      auto last = static_cast<long>(std::floor(binOffset + (x + 1) * binsPerPixel));
      if (last <= first) {
        last = first + 1;
      }
      if (last <= 0 || first >= static_cast<long>(bins)) {
        row[x] = m_colorMap[0];
        continue;
      }
      if (first < 0) {
        first = 0;
      }
      if (last > static_cast<long>(bins)) {
        last = static_cast<long>(bins);
      }

      auto peak = static_cast<float>(frame.bins[(static_cast<size_t>(first) + half) % bins]);
      for (long bin = first + 1; bin < last; ++bin) {
        float value = frame.bins[(static_cast<size_t>(bin) + half) % bins];
        if (value > peak) {
          peak = value;
        }
      }
      row[x] = m_colorMap[level(peak, scale)];
    }
  }

  uint8_t level(float db, float scale) const
  {
    float l = (db - m_minDb) * scale;
    if (!(l > 0.0f)) {        // also catches NaN, e.g. log10(0)
      return 0;
    }
    if (l >= static_cast<float>(ColorMap<Format>::LEVELS - 1)) {
      return ColorMap<Format>::LEVELS - 1;
    }
    return static_cast<uint8_t>(l);
  }

  ColorMap<Format> m_colorMap;
  uint8_t* m_pixels = nullptr;
  uint16_t m_width = 0;
  uint16_t m_height = 0;
  size_t m_strideBytes = 0;
  ScrollMode m_mode = ScrollMode::Ring;
  uint16_t m_head = 0;

  float m_minDb = -110.0f;
  float m_maxDb = -50.0f;

  bool m_hasHistory = false;
  uint32_t m_span = 0;
  double m_startFrequency = 0.0;
};
