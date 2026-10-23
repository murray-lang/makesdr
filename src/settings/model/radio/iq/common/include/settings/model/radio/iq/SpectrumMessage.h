#pragma once
#include <settings/model/message/MessageT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>

#include <samples/SampleTypes.h>
#include <settings/model/radio/PipelineId.h>


class SpectrumMessage : public MessageT<
  makesdr_SpectrumPb,
  &makesdr_SpectrumPb_msg,
  makesdr_SpectrumPayloadPb,
  makesdr_RadioPayloadType_PAYLOAD_SPECTRUM,
  makesdr_SpectrumPayloadPb_size
>
{
public:
  SpectrumMessage() = default;
  SpectrumMessage(const PayloadProto& payload) : MessageT(payload) {}
  SpectrumMessage(const SpectrumMessage& other) = default;
  SpectrumMessage& operator=(const SpectrumMessage& other);

  void initialise(PipelineId pipelineId, uint32_t sampleRate, float floor, float ceiling);

  [[nodiscard]] PipelineId pipelineId() const { return static_cast<PipelineId>(m_payload.body.pipeline_id); }
  [[nodiscard]] makesdr_SpectrumPb_bins_t& bins() { return m_payload.body.bins; }
  [[nodiscard]] const makesdr_SpectrumPb_bins_t& bins() const { return m_payload.body.bins; }
  [[nodiscard]] uint32_t sampleRate() const { return m_payload.body.sample_rate; }
  [[nodiscard]] float floor() const { return m_payload.body.floor; }
  [[nodiscard]] float ceiling() const { return m_payload.body.ceiling; }
};
