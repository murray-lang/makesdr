#pragma once

#include <samples/SampleTypes.h>
#include <dsp/window/Window.h>
#include <dsp/constants/constants.h>
#include <dsp/transforms/fft/Fft.h>

class FirKernel
{
public:
  FirKernel()
  {
    initialise();
  }
  void initialise()
  {
    m_complexCoefficients.assign(FILTER_FFT_SIZE, sdrcomplex(0, 0));
    calculateWindow();
  }

  const ComplexSamplesFft& getComplexCoefficients() { return m_complexCoefficients; }
  sdrreal getWindowAt(uint32_t i) { return m_window.at(i); }

protected:
  void calculateWindow()
  {
    m_window.resize(FIR_SIZE);
    //create Blackman-Nuttall window function for windowed sinc low pass filter design
    for(RealSamplesFir::size_type i = 0; i < FIR_SIZE; i++)
    {
      m_window.at(i) = (
          static_cast<sdrreal>(0.3635819)
          - static_cast<sdrreal>(0.4891775) * static_cast<sdrreal>(cos( (K_2PI*(sdrreal)i)/(sdrreal)(FIR_SIZE-1) ))
          + static_cast<sdrreal>(0.1365995) * static_cast<sdrreal>(cos( (2.0*K_2PI*(sdrreal)i)/(sdrreal)(FIR_SIZE-1) ))
          - static_cast<sdrreal>(0.0106411) * static_cast<sdrreal>(cos( (3.0*K_2PI*(sdrreal)i)/(sdrreal)(FIR_SIZE-1) ))
      );
    }
    //testSymetry(m_window, m_firSize, (m_firSize - 1)/2);
  }

  static const RealSamplesFft& complexToReal(const ComplexSamplesFft& complex, RealSamplesFft& real)
  {
    for (int i = 0; i < complex.size(); i++) {
      const sdrcomplex& cpx = complex.at(i);
      real.at(i) = std::hypot(cpx.real(), cpx.imag());
    }
    return real;
  }

  static void normaliseCoefficients(ComplexSamplesFft& coefficients)
  {
    sdrreal sum = 0.0;
    for (auto coeff : coefficients) {
      sum += std::hypot(coeff.real(), coeff.imag());
    }
    for (auto& coeff : coefficients) {
      coeff /= sum;  // Normalize each coefficient
    }
  }

  static void normaliseCoefficients(RealSamplesFft& coefficients)
  {
    sdrreal sum = 0.0;
    for (auto coeff : coefficients) {
      sum += coeff;
    }
    for (auto& coeff : coefficients) {
      coeff /= sum;  // Normalize each coefficient
    }
  }

  static void _normaliseCoefficients(ComplexSamplesFft& coefficients)
  {
    sdrreal maxMagnitude = 0.0;
    for (const auto& coeff : coefficients) {
      sdrreal magnitude = std::hypot(coeff.real(), coeff.imag());
      if (magnitude > maxMagnitude) {
        maxMagnitude = magnitude;
      }
    }
    if (maxMagnitude > 0.0) {
      // Scale coefficients to have a maximum magnitude of 1.0
      for (auto& coeff : coefficients) {
        coeff /= maxMagnitude;
      }
    }
  }

  static void _normaliseCoefficients(RealSamplesFft& coefficients)
  {
    sdrreal maxMagnitude = 0.0;
    for (const auto& coeff : coefficients) {
      if (coeff > maxMagnitude) {
        maxMagnitude = coeff;
      }
    }
    if (maxMagnitude > 0.0) {
      // Scale coefficients to have a maximum magnitude of 1.0
      for (auto& coeff : coefficients) {
        coeff /= maxMagnitude;
      }
    }

  }

  // static bool testSymetry(const vsdrreal& values, uint32_t length, uint32_t centreIndex)
  // {
  //   const uint32_t maxDistance = (length-1)/2;
  //   if ( centreIndex != maxDistance)
  //   {
  //     return false;
  //   }
  //   uint32_t distanceFromCentre = 1;
  //   while (distanceFromCentre < maxDistance)
  //   {
  //     const sdrreal below = values.at(centreIndex - distanceFromCentre);
  //     const sdrreal above = values.at(centreIndex + distanceFromCentre);
  //     if (below != above)
  //     {
  //       return false;
  //     }
  //     distanceFromCentre++;
  //   }
  //   return true;
  // }

protected:
  ComplexSamplesFft m_complexCoefficients;
  RealSamplesFir m_window;
  // Fft<ComplexSamplesFft> m_fft;
};
