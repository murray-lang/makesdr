#pragma once
#include "settings/model/proto/RadioSettings.pb.h"

enum class PipelineId
{
  NONE = makesdr_PipelineId_PIPELINE_NONE,
  A = makesdr_PipelineId_PIPELINE_A,
  B = makesdr_PipelineId_PIPELINE_B
};

using VfoId = PipelineId;