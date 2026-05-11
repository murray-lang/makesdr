#pragma once

#include <ResultCode.h>
#include <cstdint>

#define MAX_FIELD_NAME_LENGTH 32

extern ResultCode lookup_field_path(const char* path, uint32_t* tags_out, uint32_t max_tags);

