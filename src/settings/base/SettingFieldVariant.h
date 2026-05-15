#pragma once
#include <etl/variant.h>
#include <etl/string.h>

#define MAX_NAME_LENGTH 7
#define Max_LABEL_LENGTH 8

using NameString = etl::string<MAX_NAME_LENGTH>;
using LabelString = etl::string<Max_LABEL_LENGTH>;

using SettingFieldVariant = etl::variant<
  NameString,
  LabelString,
  int32_t,
  uint32_t,
  int64_t,
  float,
  bool,
  const char *
>;