#pragma once

#include "pb.h"

#include "pb_decode.h"
#include "pb_encode.h"
#include <ResultCode.h>

#define GET_FIELD_ID(a, alloc, rule, type, name, tag) (tag)

#define MAX_MESSAGE_DEPTH 10
namespace ProtobufIo
{
  template <typename MessageType>
  ResultCode readProtobuf(
    const uint8_t *buffer,
    size_t msg_length,
    const pb_msgdesc_t* fields,
    const MessageType& InitZero,
    MessageType& message
  ) {
    // This is critical to ensure no garbage data remains.
    message = InitZero;
    pb_istream_t stream = pb_istream_from_buffer(buffer, msg_length);

    if (!pb_decode(&stream, fields, &message)) {
      return ResultCode::ERR_SETTING_READING_PROTOBUF;
    }
    return ResultCode::OK;
  }

  template <typename MessageType>
  ResultCode writeProtobuf(
    uint8_t *buffer,
    size_t buffer_size,
    size_t* bytes_written,
    const pb_msgdesc_t* fields,
    const MessageType& message
  ) {
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, buffer_size);

    if (!pb_encode(&stream, fields, &message)) {
      return ResultCode::ERR_SETTING_WRITING_PROTOBUF;
    }
    *bytes_written = stream.bytes_written;
    return ResultCode::OK;
  }
}