#pragma once

#include <etl/vector.h> // "Sample" vectors (i.e., audio, I/Q) are always ETL
#include <complex> // complex is always std::

using sdrreal = float;
using sdrcomplex = std::complex<sdrreal>;

#define FIR_SIZE ((FFT_SIZE)/2 +1)

using ComplexSamplesBuffer = etl::vector<sdrcomplex, PIPELINE_BUFFER_LENGTH>;
using RealSamplesBuffer = etl::vector<sdrreal, PIPELINE_BUFFER_LENGTH*2>;

using ComplexSamplesFft = etl::vector<sdrcomplex, FFT_SIZE>;
using RealSamplesFft = etl::vector<sdrreal, FFT_SIZE>;
using ComplexSamplesFir = etl::vector<sdrcomplex, FIR_SIZE>;
using RealSamplesFir = etl::vector<sdrreal, FIR_SIZE>;