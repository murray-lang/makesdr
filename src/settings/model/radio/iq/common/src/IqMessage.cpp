#include "IqMessage.h"
#include <cstring>


IqMessage::IqMessage(PipelineId pipelineId, const ComplexSamplesBuffer& iq, uint32_t length, uint32_t sampleRate)
{
  initialise(pipelineId, iq, length, sampleRate);
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
IqMessage::initialise(PipelineId pipelineId, const ComplexSamplesBuffer& iq, uint32_t length, uint32_t sampleRate)
{
  m_payload.body.pipeline_id = static_cast<makesdr_PipelineId>(pipelineId);
  const uint32_t count = std::min(static_cast<uint32_t>(iq.size()), length);
  std::memcpy(m_payload.body.interleaved, iq.data(), count * sizeof(sdrcomplex));
  m_payload.body.interleaved_count = static_cast<pb_size_t>(count * 2);
  m_payload.body.sample_rate = sampleRate;
}