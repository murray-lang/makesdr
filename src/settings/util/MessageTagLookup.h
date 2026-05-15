#pragma once

#include <ResultCode.h>
#include <cstdint>

#define MAX_FIELD_NAME_LENGTH 32

// Field name to tag mappings (organized by message type)
struct FieldEntry {
  const char* name;
  int32_t tag;
  const FieldEntry* submsg;  // Pointer to submessage field table, or nullptr
};

extern const FieldEntry radio_fields[];

extern ResultCode lookup_field_path(const char* path, uint32_t* tags_out, uint32_t max_tags);

