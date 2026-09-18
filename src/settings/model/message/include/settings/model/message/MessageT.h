#pragma once

#include "IMessage.h"
#include <ResultCode.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/proto/ProtobufIo.h>
#include "MessageTraverser.h"
#include "PayloadType.h"
#include "PayloadPurpose.h"
#include "PayloadSource.h"

template<
  typename MessagePbType,
  const pb_msgdesc_t* descriptor,
  typename PayloadPbType,
  makesdr_RadioPayloadType payloadTypeEnum,
  int _payloadSize
>
class MessageT : public IMessage
{
public:
  using Proto = MessagePbType;
  using PayloadProto = PayloadPbType;
  static const int payloadType = static_cast<int>(payloadTypeEnum);

  MessageT()
    : m_deemComplete(false)
    , m_payload{0}
    , m_traverser(&m_payload.body, descriptor)
  {
    m_payload.header.payloadType = payloadTypeEnum;
    m_payload.has_header = true;
    m_payload.has_body = true;
  }

  MessageT(const PayloadProto& payload, bool complete = true) noexcept
    : m_deemComplete(complete)
    , m_payload(payload)
    , m_traverser(&m_payload.body, descriptor)
  {
  }

  MessageT(const MessageT& other) noexcept
    : m_deemComplete(other.m_deemComplete)
    , m_payload(other.m_payload)
    , m_traverser(&m_payload.body, descriptor)
  {
  }

  MessageT(MessageT&& other) noexcept
    : m_deemComplete(other.m_deemComplete)
    , m_payload(other.m_payload)
    , m_traverser(&m_payload.body, descriptor)

  {
  }

  ~MessageT() override = default;

  PayloadType getPayloadType() override { return static_cast<PayloadType>(payloadType); }
  int payloadSize() override { return _payloadSize; }

  PayloadPbType& payload() { return m_payload; }
  [[nodiscard]] const PayloadPbType& payload() const { return m_payload; }

  MessagePbType& body() { return m_payload.body; }
  [[nodiscard]] const MessagePbType& body() const { return m_payload.body; }
  [[nodiscard]] PayloadSource source() const { return static_cast<PayloadSource>(m_payload.header.source); }
  void setSource(PayloadSource source) { m_payload.header.source = static_cast<makesdr_RadioPayloadSource>(source); }

  [[nodiscard]] PayloadPurpose purpose() const { return static_cast<PayloadPurpose>(m_payload.header.purpose); }
  void setPurpose(PayloadPurpose purpose) { m_payload.header.purpose = static_cast<makesdr_RadioPayloadPurpose>(purpose); }

  void copyTo(MessagePbType& out) const { out = m_payload.body; }

  void deemComplete(bool deemComplete) { m_deemComplete = deemComplete; }
  [[nodiscard]] bool deemComplete() const { return m_deemComplete; }

  ResultCode setAllFieldsPresence(bool present)
  {
    return m_traverser.setAllFieldsPresence(present);
  }

  ResultCode writeProtobuf(
    makesdr_RadioPayloadPurpose purpose,
    uint8_t *buffer,
    size_t buffer_size,
    size_t* bytes_written
    ) override
  {
    if (buffer_size < _payloadSize) {
      return ResultCode::ERR_PROTOBUF_BUFFER_TOO_SMALL;
    }
    makesdr_RadioPayloadPurpose savePurpose = m_payload.header.purpose;
    m_payload.header.purpose = purpose;
    ResultCode rc = ProtobufIo::writeProtobuf<PayloadPbType>(
      buffer,
      _payloadSize,
      bytes_written,
      descriptor,
      m_payload
      );
    m_payload.header.purpose = savePurpose;
    return rc;
  }

  ResultCode readProtobuf(const uint8_t *buffer, size_t msg_length) override
  {
    return ProtobufIo::readProtobuf<PayloadPbType>(
      buffer,
      msg_length,
      descriptor,
      {0},
      m_payload
      );
  }

  ResultCode applyFieldUpdate(const FieldUpdate &settingUpdate) override
  {
    return m_traverser.updateField(settingUpdate);
  }

  ResultCode updateField(const FieldPath &path, const FieldUpdateVariant &value) override
  {
    return m_traverser.updateField(path, value);
  }

  ResultCode getField(const FieldPath &path, FieldUpdateVariant &value) const override
  {
    return m_traverser.getField(path, value);
  }

  ResultCode getField(
    const FieldPath &path,
    FieldUpdateVariant &value,
    bool mustHave,
    bool parentsMustHave,
    bool& retrieved
  )
  {
    return m_traverser.getField(path, value, mustHave, parentsMustHave, retrieved);
  }

  ResultCode setFieldPresence(const FieldPath &path, bool present)
  {
    return m_traverser.setFieldPresence(path, present);
  }

  ResultCode mergePresentFields(const void* pRhsMessage)
  {
    return m_traverser.mergePresentFields(pRhsMessage);
  }

  ResultCode replace(IMessage& other, bool deemComplete) override
  {
    if (other.getPayloadType() != static_cast<PayloadType>(payloadTypeEnum)) {
      return ResultCode::ERR_PROTOBUF_PAYLOAD_MISMATCH;
    }
    const auto& otherAsSameType = static_cast<const MessageT&>(other);
    return replace(otherAsSameType.body(), deemComplete);
  }

  ResultCode merge(IMessage& other) override
  {
    if (other.getPayloadType() != static_cast<PayloadType>(payloadTypeEnum)) {
      return ResultCode::ERR_PROTOBUF_PAYLOAD_MISMATCH;
    }
    const auto& otherAsSameType = static_cast<const MessageT&>(other);
    return merge(otherAsSameType.body());
  }

  virtual ResultCode replace(const MessagePbType& update, bool deemComplete)
  {
    m_payload.body = update;
    m_deemComplete = deemComplete;
    if (deemComplete) {
      setAllFieldsPresence(true);
    }
    return ResultCode::OK;
  };

  virtual ResultCode merge(const MessagePbType& update)
  {
    return m_traverser.mergePresentFields(&update);
  }

protected:
  bool m_deemComplete;
  PayloadPbType m_payload;

  MessageTraverser m_traverser;
};