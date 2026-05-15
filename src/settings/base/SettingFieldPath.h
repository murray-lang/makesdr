#pragma once
#include <etl/vector.h>

#define MAX_FIELD_PATH_LENGTH 6

using SettingFieldPath = etl::vector<uint32_t, MAX_FIELD_PATH_LENGTH>;
