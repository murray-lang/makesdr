#include "IqMessage.h"
#include <cstring>


IqMessage::IqMessage(const ComplexSamplesBuffer& iq, uint32_t length, uint32_t sampleRate)
{
  initialise(iq, length, sampleRate);
}

IqMessage&
IqMessage::operator=(const IqMessage& other)
{
  if (this != &other) {
    m_payload = other.m_payload;
  }
  return *this;
}

void
IqMessage::initialise(const ComplexSamplesBuffer& iq, uint32_t length, uint32_t sampleRate)
{
  const uint32_t count = std::min(static_cast<uint32_t>(iq.size()), length);
  std::memcpy(m_payload.body.interleaved, iq.data(), count * sizeof(sdrcomplex));
  m_payload.body.interleaved_count = static_cast<pb_size_t>(count * 2);
  m_payload.body.sample_rate = sampleRate;
}