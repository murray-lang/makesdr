#pragma once
#include <settings/model/message/MessageT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>

#include "samples/SampleTypes.h"


class IqMessage : public MessageT<
  makesdr_IqPb,
  &makesdr_IqPb_msg,
  makesdr_IqPayloadPb,
  makesdr_RadioPayloadType_PAYLOAD_IQ,
  makesdr_IqPayloadPb_size
>
{
public:
  IqMessage() = default;
  IqMessage(const PayloadProto& payload) : MessageT(payload) {}
  IqMessage(const IqMessage& other) = default;
  IqMessage(const ComplexSamplesBuffer& iq, uint32_t length, uint32_t sampleRate);
  IqMessage& operator=(const IqMessage& other);

  void initialise(const ComplexSamplesBuffer& iq, uint32_t length, uint32_t sampleRate);

  [[nodiscard]] const float* data() const { return m_payload.body.interleaved; }
  [[nodiscard]] uint32_t length() const { return m_payload.body.interleaved_count; }
  [[nodiscard]] uint32_t sampleRate() const { return m_payload.body.sample_rate; }
};
