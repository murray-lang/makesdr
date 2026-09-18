#pragma once
#include <cstdint>
#include "settings/model/proto/RadioPayloads.pb.h"

enum PayloadPurpose : uint16_t
{
  PURPOSE_NONE = makesdr_RadioPayloadPurpose_PURPOSE_NONE,
  PURPOSE_MERGE = makesdr_RadioPayloadPurpose_PURPOSE_MERGE,
  PURPOSE_REPLACE = makesdr_RadioPayloadPurpose_PURPOSE_REPLACE
};
