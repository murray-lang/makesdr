#include "dsp/filters/fir/kernels/BandPassFirKernel.h"
#include <samples/SampleTypes.h>
// #include <QDebug>


const ComplexSamplesFft&
BandPassFirKernel::configureComplex(int32_t freqLoCut, int32_t freqHiCut, int32_t offset, uint32_t sampleRate)
{
  auto sampleRateReal = static_cast<sdrreal>(sampleRate);
  sdrreal loCutRate = static_cast<sdrreal>(freqLoCut + offset) / sampleRateReal;
  sdrreal hiCutRate = static_cast<sdrreal>(freqHiCut + offset) / sampleRateReal;
  sdrreal signal = (hiCutRate - loCutRate) / 2.0f;
  sdrreal localOsc = K_2PI * (hiCutRate + loCutRate) / 2.0;
  int32_t centreIndex = static_cast<int32_t>(FIR_SIZE-1)/2;
  int32_t outStartIndex = (static_cast<int32_t>(FFT_SIZE)/2) - centreIndex;


  ComplexSamplesFft complexSincPulse(
      FFT_SIZE,
      sdrcomplex(static_cast<sdrreal>(0.0), static_cast<sdrreal>(0.0))
  );
  RealSamplesFft realSincPulse(FFT_SIZE, static_cast<sdrreal>(0.0));
//  for (auto& item : m_complexSincPulse) {
//    item = sdrcomplex(0.0f, 0.0f);
//  }

//  for(int32_t i = 0; i < m_firSize; i++) {
  for(int32_t i = 0; i < FIR_SIZE; i++) {

    auto x = static_cast<sdrreal>(i - centreIndex);

    sdrreal z;
    if (i == centreIndex) //deal with odd size filter singularity where sin(0)/0==1
    {
      z = static_cast<sdrreal>(2.0) * signal; // i.e. (K_2PI * signal) / K_PI
    } else {
      z = static_cast<sdrreal>(sin((sdrreal)2.0 * (sdrreal)K_PI * x * signal) / ((sdrreal) K_PI * x)); // * m_window.at(i);
      //z = static_cast<sdrreal>(sin((sdrreal)2.0 *K_PI * x * signal) / x); // * m_window.at(i);

    }
    // sdrreal window = BLACKMAN(i, static_cast<sdrreal>(FIR_SIZE)); //m_window.at(i) - m_window[0];
    z *= m_window.at(i); // window; //HANNING(i, m_firSize);
    realSincPulse.at( i ) = z;
    // qDebug() << z;

    //shift lowpass filter coefficients in frequency by (hicut+lowcut)/2 to form bandpass filter anywhere in range
    // (also scales by 1/FFTsize since inverse FFT routine scales by FFTsize)
//    m_complexSincPulse.at(i ) = sdrcomplex(
//        z * static_cast<sdrreal>(cos(localOsc * x)),
//        z * static_cast<sdrreal>(sin(localOsc * x))
//    );
  }
  normaliseCoefficients(realSincPulse);
  for (int32_t i = 0; i < FIR_SIZE; i++) {
    auto x = static_cast<sdrreal>(i - centreIndex);
    sdrreal z = realSincPulse.at(i);
    complexSincPulse.at(i) = sdrcomplex(
        z * static_cast<sdrreal>(cos(localOsc * x)),
        z * static_cast<sdrreal>(sin(localOsc * x))
    );
  }
  Fft<ComplexSamplesFft>fft(WindowType::NONE);
  fft.transform(complexSincPulse, m_complexCoefficients, FFT_SIZE, true, true);
  // static int times = 0;
  // times++;
  // if (times == 2) {
  //   for (int i = 0; i < FFT_SIZE; i++) {
  //     qDebug() << std::abs(m_complexCoefficients.at(i));
  //   }
  // }
  return m_complexCoefficients;
}
