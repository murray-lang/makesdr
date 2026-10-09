#include "FftMessage.h"
#include <cstring>


FftMessage&
FftMessage::operator=(const FftMessage& other)
{
  if (this != &other) {
    m_payload = other.m_payload;
  }
  return *this;
}

void
FftMessage::initialise(PipelineId pipelineId, uint32_t sampleRate, float floor, float ceiling)
{
  m_payload.body.pipeline_id = static_cast<makesdr_PipelineId>(pipelineId);
  m_payload.body.sample_rate = sampleRate;
  m_payload.body.floor = floor;
  m_payload.body.ceiling = ceiling;
}