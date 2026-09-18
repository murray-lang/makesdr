#pragma once

#include <ResultCode.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include "FieldPath.h"
#include "FieldUpdate.h"
#include "FieldUpdateSink.h"
#include "FieldUpdateVariant.h"
#include "PayloadType.h"


class IMessage : public FieldUpdateSink
{
public:
  ~IMessage() override = default;

  virtual PayloadType getPayloadType() = 0;
  virtual int payloadSize() = 0;

  virtual ResultCode writeProtobuf(
    makesdr_RadioPayloadPurpose purpose,
    uint8_t *buffer,
    size_t buffer_size,
    size_t* bytes_written
    ) = 0;

  virtual ResultCode readProtobuf(const uint8_t *buffer, size_t msg_length) = 0;

  virtual ResultCode updateField(const FieldPath &path, const FieldUpdateVariant &value) = 0;
  virtual ResultCode getField(const FieldPath &path, FieldUpdateVariant &value) const = 0;

  // virtual ResultCode applyFieldUpdate(const FieldUpdate &fieldUpdate) = 0;

  virtual ResultCode replace(IMessage& other, bool deemComplete) = 0;
  virtual ResultCode merge(IMessage& other) = 0;
};