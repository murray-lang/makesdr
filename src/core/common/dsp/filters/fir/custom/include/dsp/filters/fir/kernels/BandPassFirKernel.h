//
// Created by murray on 18/02/25.
//

#pragma once


#include "FirKernel.h"

class BandPassFirKernel : public FirKernel
{
public:
  BandPassFirKernel() = default;

  void configure(int32_t freqLoCut, int32_t freqHiCut, int32_t offset, uint32_t sampleRate)
  {
    configureComplex(freqLoCut, freqHiCut, offset, sampleRate);
  }

protected:
  const ComplexSamplesFft& configureComplex(int32_t freqLoCut, int32_t freqHiCut, int32_t offset, uint32_t sampleRate);


};
