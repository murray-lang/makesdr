#pragma once
#include <settings/model/message/MessageT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>

#include <samples/SampleTypes.h>
#include <settings/model/radio/PipelineId.h>


class FftMessage : public MessageT<
  makesdr_FftPb,
  &makesdr_FftPb_msg,
  makesdr_FftPayloadPb,
  makesdr_RadioPayloadType_PAYLOAD_FFT,
  makesdr_FftPayloadPb_size
>
{
public:
  FftMessage() = default;
  FftMessage(const PayloadProto& payload) : MessageT(payload) {}
  FftMessage(const FftMessage& other) = default;
  FftMessage& operator=(const FftMessage& other);

  void initialise(PipelineId pipelineId, uint32_t sampleRate, float floor, float ceiling);

  [[nodiscard]] PipelineId pipelineId() const { return static_cast<PipelineId>(m_payload.body.pipeline_id); }
  [[nodiscard]] makesdr_FftPb_bins_t& bins() { return m_payload.body.bins; }
  [[nodiscard]] const makesdr_FftPb_bins_t& bins() const { return m_payload.body.bins; }
  [[nodiscard]] uint32_t sampleRate() const { return m_payload.body.sample_rate; }
  [[nodiscard]] float floor() const { return m_payload.body.floor; }
  [[nodiscard]] float ceiling() const { return m_payload.body.ceiling; }
};
