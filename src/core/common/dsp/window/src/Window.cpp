#include "dsp/window/Window.h"
#include <dsp/constants/constants.h>
#include <cmath>

// All arithmetic is kept in sdrreal (float): the Cortex-M4 FPU is single precision
// only, so any double literal or ::cos(double) here falls back to soft-float.
static constexpr sdrreal kTwoPi = static_cast<sdrreal>(2.0 * K_PI);
static constexpr sdrreal kFourPi = static_cast<sdrreal>(4.0 * K_PI);

static inline sdrreal windowPhase(uint32_t i, uint32_t N)
{
  return static_cast<sdrreal>(i) / static_cast<sdrreal>(N - 1);
}

WindowFunction window_none = [] (uint32_t i, uint32_t N) -> sdrreal
{
  return 1.0f;
};

WindowFunction window_hanning = [] (uint32_t i, uint32_t N) -> sdrreal
{
  return 0.5f * (1.0f - std::cos(kTwoPi * windowPhase(i, N)));
};

WindowFunction window_hamming = [] (uint32_t i, uint32_t N) -> sdrreal
{
  return 0.54f - 0.46f * std::cos(kTwoPi * windowPhase(i, N));
};

WindowFunction window_blackman = [] (uint32_t i, uint32_t N) -> sdrreal
{
  const sdrreal x = windowPhase(i, N);
  return 0.42f - 0.5f * std::cos(kTwoPi * x) + 0.08f * std::cos(kFourPi * x);
};
