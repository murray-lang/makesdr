#include "iq/modulation/AmDemodulator.h"

#include <algorithm>
#include <QDebug>

uint32_t
AmDemodulator::processSamples(
    const ComplexSamplesBuffer& in,
    RealSamplesBuffer& out,
    uint32_t inputLength)
{
  if (inputLength == 0)
  {
    return 0;
  }

  for(uint32_t i=0; i<inputLength; i++)
  {
    ampmodem_demodulate(m_demodulator, in[i], &out[i]);
  }
  // sdrreal sum = 0.0;
  // for(uint32_t i=0; i<inputLength; i++)
  // {
  //   //calculate instantaneous power magnitude of pInData which is I*I + Q*Q
  //   sdrcomplex iq = in.at(i);
  //   sdrreal mag = std::abs(iq);
  //   sum += mag;
  //   out.at(i) = mag;
  // }
  // sdrreal avg = sum / static_cast<sdrreal>(inputLength);
  // // qDebug() << "AM avg: " << avg;
  // std::for_each(out.begin(), out.begin() + inputLength, [avg](sdrreal& v) {
  //     v -= avg;
  // });
  return inputLength;
}
