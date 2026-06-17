#pragma once
#include <CrossPlatformTypes.h>
#include <etl/variant.h>
#include <etl/string.h>

#ifdef USE_ETL
using NameString = etl::string<MAX_NAME_LENGTH>;
using LabelString = etl::string<MAX_MODE_LABEL_LENGTH>;

using SettingUpdateVariant = variant<
  monostate,
  NameString,
  LabelString,
  int32_t,
  uint32_t,
  int64_t,
  float,
  bool,
  const char *
>;

#else
using NameString = std::string;
using LabelString = std::string;

using SettingUpdateVariant = variant<
  monostate,
  std::string,
  int32_t,
  uint32_t,
  int64_t,
  float,
  bool,
  const char *
>;
#endif

