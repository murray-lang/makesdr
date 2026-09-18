#pragma once
#include "MessageT.h"
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include "FieldUpdateMeaning.h"
#include "PayloadSource.h"
#include "StringRef.h"

class FieldUpdate;

class FieldUpdateMessage : public MessageT<
  makesdr_FieldUpdatePb,
  &makesdr_FieldUpdatePb_msg,
  makesdr_FieldUpdatePayloadPb,
  makesdr_RadioPayloadType_PAYLOAD_FIELD_UPDATE,
  makesdr_FieldUpdatePayloadPb_size
>
{
public:
  FieldUpdateMessage() = default;
  FieldUpdateMessage(const PayloadProto& payload) : MessageT(payload) {}
  FieldUpdateMessage(const FieldUpdate& field, PayloadSource source);
  FieldUpdateMessage(const FieldUpdateMessage& rhs) = default;

  FieldUpdateMessage& operator=(const FieldUpdateMessage& rhs)
  {
    if (this != &rhs) {
      m_payload = rhs.m_payload;
    }
    return *this;
  }

  [[nodiscard]] const uint32_t* path() const { return static_cast<const uint32_t*>(m_payload.body.path); }
  [[nodiscard]] FieldUpdateMeaning meaning() const
  {
    return static_cast<FieldUpdateMeaning>(m_payload.body.meaning);
  }
  bool getValue(FieldUpdateVariant& var) const;
  bool setValue(const FieldUpdateVariant& var);
  [[nodiscard]] bool isIndirect() const { return m_payload.body.is_indirect; }
  [[nodiscard]] bool needsAutoComplete() const
  {
    return m_payload.body.is_auto_complete;
  }
  [[nodiscard]] bool isFinal() const { return m_payload.body.is_final; }
};