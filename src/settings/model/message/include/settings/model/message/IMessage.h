#pragma once

#include <ResultCode.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include "PayloadType.h"


class IMessage
{
public:
  virtual ~IMessage() = default;

  virtual PayloadType getPayloadType() = 0;
  virtual int payloadSize() = 0;

  virtual ResultCode writeProtobuf(
    makesdr_RadioPayloadPurpose purpose,
    uint8_t *buffer,
    size_t buffer_size,
    size_t* bytes_written
    ) = 0;

  virtual ResultCode readProtobuf(const uint8_t *buffer, size_t msg_length) = 0;
};