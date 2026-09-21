//
// Created by murray on 18/02/25.
//

#pragma once


#include "FirKernel.h"

class LowPassFirKernel : public FirKernel {
public:
  LowPassFirKernel() = default;

  void configure(int32_t freqHiCut, int32_t offset, uint32_t sampleRate)
  {
    configureComplex(freqHiCut, offset, sampleRate);
  }

protected:
  const ComplexSamplesFft& configureComplex(int32_t freqHiCut, int32_t offset, uint32_t sampleRate);

};

