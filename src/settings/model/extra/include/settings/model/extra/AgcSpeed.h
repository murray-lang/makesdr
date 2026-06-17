#pragma once
#include "settings/model/proto/RadioSettings.pb.h"

enum class AgcSpeed
{
  OFF = makesdr_AgcSpeed_AGC_OFF,
  SLOW = makesdr_AgcSpeed_AGC_SLOW,
  MEDIUM = makesdr_AgcSpeed_AGC_MEDIUM,
  FAST = makesdr_AgcSpeed_AGC_FAST,
  DEFAULT = FAST
};