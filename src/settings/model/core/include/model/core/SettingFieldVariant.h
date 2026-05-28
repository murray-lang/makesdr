#pragma once
#include <etl/variant.h>
#include <etl/string.h>


using NameString = etl::string<MAX_NAME_LENGTH>;
using LabelString = etl::string<MAX_MODE_LABEL_LENGTH>;

using SettingFieldVariant = etl::variant<
  etl::monostate,
  NameString,
  LabelString,
  int32_t,
  uint32_t,
  int64_t,
  float,
  bool,
  const char *
>;