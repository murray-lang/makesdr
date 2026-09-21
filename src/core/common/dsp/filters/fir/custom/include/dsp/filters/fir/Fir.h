#pragma once

#include <CrossPlatformTypes.h>
#include <samples/SampleTypes.h>
#include "kernels/BandPassFirKernel.h"
// #include "kernels/BandStopFirKernel.h"
// #include "kernels/LowPassFirKernel.h"
#include <dsp/transforms/fft/Fft.h>

template<class kernel>
class Fir
{
public:
  explicit Fir()
    : m_kernel()
    , m_fft(WindowType::HANNING)
  {
  }
  virtual ~Fir() = default;

  kernel& getKernel() { return m_kernel; }

  void configure(int32_t freqLoCut, int32_t freqHiCut, int32_t offset, uint32_t sampleRate)
  {
    m_kernel.configure(freqLoCut, freqHiCut, offset, sampleRate);
  }

// private:
  void filter(ComplexSamplesFft& input, ComplexSamplesFft& output)
  {
    applyFftCoefficients(input, output);
  }


  void filter(const ComplexSamplesFft& input, ComplexSamplesFft& output)
  {
    applyFftCoefficients(input, output);
  }
protected:

  void applyFftCoefficients(const ComplexSamplesFft& input, ComplexSamplesFft& output)
  {
    m_fft.transform(input, output, FFT_SIZE, true, false);
    multiplyByCoefficients(output, output);
    m_fft.transform(output, output, FFT_SIZE, false, true);

    // uint32_t inputSize = input.size();
    // ComplexSamplesFft localInput, localOutput;
    // localInput.resize(inputSize);
    // localOutput.resize(inputSize);
    // m_fft.transform(input, localInput, FFT_SIZE, true, false);
    //
    // multiplyByCoefficients(localInput, localOutput);
    //
    // m_fft.transform(localOutput, output, FFT_SIZE, false, true);
  }

  void multiplyByCoefficients(const ComplexSamplesFft& values, ComplexSamplesFft& result)
  {
    const ComplexSamplesFft& coefficients = m_kernel.getComplexCoefficients();
    std::transform(
        std::begin(values),
        std::end(values),
        std::begin(coefficients),
        std::begin(result),
        std::multiplies<>()
    );
  }

  void multiplyByCoefficients(const RealSamplesFft& values, RealSamplesFft& result)
  {
    std::transform(
        std::begin(values),
        std::end(values),
        std::begin(m_kernel.getRealCoefficients()),
        std::begin(result),
        std::multiplies<>()
    );
  }

private:
  kernel m_kernel;
  Fft<ComplexSamplesFft> m_fft;
};

using BandPassFilter = Fir<BandPassFirKernel>;
// using BandStopFilter = Fir<BandStopFirKernel>;
// using LowPassFilter = Fir<LowPassFirKernel>;
