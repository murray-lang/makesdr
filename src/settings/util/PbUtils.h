#pragma once
#include "pb.h"
#include "pb_common.h"
#include <vector>
#include <string>

#include "pb_decode.h"
#include "RadioSettings.pb.h"

#define GET_FIELD_ID(a, alloc, rule, type, name, tag) (tag)

#define MAX_MESSAGE_DEPTH 10
namespace PbUtils
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
}