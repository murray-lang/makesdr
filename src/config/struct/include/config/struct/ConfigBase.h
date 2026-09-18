#pragma once
#include <cstdint>


#ifdef USE_ETL
#include <etl/string.h>
#include <etl/vector.h>
#include "etl/variant.h"
#include "etl/optional.h"

using etl::optional;
#else
#include <string>
#include <vector>
#include <variant>
#include <optional>

using std::optional;
#endif

#include <ResultCode.h>

#define TYPE_LENGTH_INCL_0 24

namespace Config
{
#ifdef USE_ETL
  using TypeString = etl::string<TYPE_LENGTH_INCL_0>;
  using FieldPathString = etl::string<MAX_DOTTED_STRING_LENGTH>;
  using FieldPathTags = etl::vector<uint32_t, MAX_FIELD_PATH_LENGTH>;
#else
  using TypeString = std::string;
  using FieldPathString = std::string;
  using FieldPathTags = std::vector<uint32_t>;
#endif


  struct Alternative
  {
    TypeString type;
  };
}
