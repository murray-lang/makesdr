#pragma once
#include "ResultCode.h"
#include "settings/model/proto/ProtobufIo.h"
#include "settings/model/proto/RadioPayloads.pb.h"

class RadioPayloadBase
{
public:
  RadioPayloadBase() : m_raw(makesdr_RadioPayloadBasePb_init_zero) {}

  [[nodiscard]] makesdr_RadioPayloadType type() const { return m_raw.header.payloadType; }

  ResultCode readProtobuf(const uint8_t *buffer, size_t msg_length)
  {
    return ProtobufIo::readProtobuf<makesdr_RadioPayloadBasePb>(
      buffer,
      makesdr_RadioPayloadBasePb_size,
      &makesdr_RadioPayloadBasePb_msg,
      makesdr_RadioPayloadBasePb_init_zero,
      m_raw
    );
  }

protected:
  makesdr_RadioPayloadBasePb m_raw;
};
