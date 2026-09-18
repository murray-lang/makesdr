#pragma once
#include <CrossPlatformTypes.h>

#ifdef USE_ETL
#include <etl/variant.h>
#include <etl/string.h>
using NameString = etl::string<MAX_NAME_LENGTH>;
using LabelString = etl::string<MAX_MODE_LABEL_LENGTH>;

using FieldUpdateVariant = variant<
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

using FieldUpdateVariant = variant<
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

