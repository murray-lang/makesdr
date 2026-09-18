#pragma once
#include <cstdint>
#include "settings/model/proto/RadioPayloads.pb.h"

enum PayloadSource : uint16_t
{
  SOURCE_UNKNOWN = makesdr_RadioPayloadSource_SOURCE_UNKNOWN,
  SOURCE_BACK_END = makesdr_RadioPayloadSource_SOURCE_BACK_END,
  SOURCE_FRONT_END = makesdr_RadioPayloadSource_SOURCE_FRONT_END
};
