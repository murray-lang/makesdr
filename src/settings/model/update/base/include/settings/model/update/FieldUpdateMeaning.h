#pragma once
#include <settings/model/proto/RadioSettings.pb.h>

enum FieldUpdateMeaning
{
  NONE = makesdr_FieldUpdateMeaning_MEANING_NONE,
  VALUE = makesdr_FieldUpdateMeaning_MEANING_VALUE,
  DELTA = makesdr_FieldUpdateMeaning_MEANING_DELTA
};