#pragma once
#include <settings/model/message/MessageT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include "PipelineId.h"

class RxMeteringMessage : public MessageT<
  makesdr_RxMeteringPb,
  &makesdr_RxMeteringPb_msg,
  makesdr_RxMeteringPayloadPb,
  makesdr_RadioPayloadType_PAYLOAD_RX_METERING,
  makesdr_RxMeteringPayloadPb_size
>
{
public:
  RxMeteringMessage() = default;
  RxMeteringMessage(const PayloadProto& payload) : MessageT(payload) {}

  RxMeteringMessage& operator=(const RxMeteringMessage& rhs)
  {
    if (this != &rhs) {
      m_payload = rhs.m_payload;
    }
    return *this;
  }

  void initialise(PipelineId pipelineId, float rssiDbFs, float agcGainDb)
  {
    m_payload.body.pipeline_id = static_cast<makesdr_PipelineId>(pipelineId);
    m_payload.body.rssiDbFs = rssiDbFs;
    m_payload.body.agcGainDb = agcGainDb;
  }

  [[nodiscard]] PipelineId pipelineId() const { return static_cast<PipelineId>(m_payload.body.pipeline_id); }
  [[nodiscard]] float rssiDbFs() const { return m_payload.body.rssiDbFs; }
  [[nodiscard]] float agcGainDb() const { return m_payload.body.agcGainDb; }
};
