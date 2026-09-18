#pragma once
#include <cstdint>

#ifdef USE_ETL
#include <etl/vector.h>

using FieldPath = etl::vector<uint32_t, MAX_FIELD_PATH_LENGTH>;
#else
#include <vector>
using FieldPath = std::vector<uint32_t>;
#endif
