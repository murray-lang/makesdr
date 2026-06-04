#pragma once
#include "settings/model/proto/RadioSettings.pb.h"

enum class AgcSpeed
{
  OFF = RadioSettings_AgcSpeed_AGC_OFF,
  SLOW = RadioSettings_AgcSpeed_AGC_SLOW,
  MEDIUM = RadioSettings_AgcSpeed_AGC_MEDIUM,
  FAST = RadioSettings_AgcSpeed_AGC_FAST,
  DEFAULT = FAST
};