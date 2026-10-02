#include "iq/correction/DcShift.h"

DcShift::DcShift()
  : m_shift(0.0, 00)
{}

DcShift::DcShift(const sdrcomplex& shift)
  : m_shift(shift)
{}

uint32_t
DcShift::processSamples(ComplexPingPongBuffers& buffers, uint32_t inputLength)
{
  const ComplexSamplesBuffer& input = buffers.input();
  ComplexSamplesBuffer& output = buffers.output();

  for (uint32_t i = 0; i < inputLength; i++) {
    output[i] = input[i] + m_shift;
  }
  return inputLength;
}

DcShift&
DcShift::setShift(const sdrcomplex& shift)
{
  m_shift = shift;
  return *this;
}

const sdrcomplex&
DcShift::getShift() const
{
  return m_shift;
}