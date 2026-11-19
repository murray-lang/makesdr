#pragma once

#include <etl/vector.h> // "Sample" vectors (i.e., audio, I/Q) are always ETL
#include <complex> // complex is always std::

using sdrreal = float;
using sdrcomplex = std::complex<sdrreal>;

#define FIR_SIZE ((FILTER_FFT_SIZE)/2 +1)

using ComplexSamplesBuffer = etl::vector<sdrcomplex, PIPELINE_BUFFER_LENGTH>;
using RealSamplesBuffer = etl::vector<sdrreal, PIPELINE_BUFFER_LENGTH>;

// Demodulated/microphone audio. Sized separately from the IQ pipeline buffers so that targets whose
// audio packets are much smaller than their IQ packets (e.g. STM32) don't pay for IQ-sized storage.
using AudioSamplesBuffer = etl::vector<sdrreal, AUDIO_BUFFER_LENGTH>;

// Capacity-erased view of any etl::vector<sdrreal, N>. Use for interfaces that accept real sample
// buffers of differing capacities (e.g. AudioSink, which carries both audio and sound-card IQ).
// Element access has no extra cost: etl::ivector is the non-virtual base of etl::vector.
using RealSamplesRef = etl::ivector<sdrreal>;

using ComplexSamplesFft = etl::vector<sdrcomplex, FILTER_FFT_SIZE>;
using RealSamplesFft = etl::vector<sdrreal, FILTER_FFT_SIZE>;
using ComplexSamplesFir = etl::vector<sdrcomplex, FIR_SIZE>;
using RealSamplesFir = etl::vector<sdrreal, FIR_SIZE>;

using ComplexMonitorFft = etl::vector<sdrcomplex, MONITOR_FFT_SIZE>;
using RealMonitorFft = etl::vector<sdrreal, MONITOR_FFT_SIZE>;