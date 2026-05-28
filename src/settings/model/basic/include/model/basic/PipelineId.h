#pragma once
#include "model/proto/RadioSettings.pb.h"

enum class PipelineId
{
  NONE = RadioSettings_PipelineId_PIPELINE_NONE,
  A = RadioSettings_PipelineId_PIPELINE_A,
  B = RadioSettings_PipelineId_PIPELINE_B
};

using VfoId = PipelineId;