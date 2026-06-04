#pragma once

#include "AutoCompleteTrigger.h"
#include <cstdint>

#define MAX_FIELD_NAME_LENGTH 32

// Field name to tag mappings (organized by message type)
struct FieldEntry {
  const char* name;
  int32_t tag;
  const FieldEntry* submsg;  // Pointer to submessage field table, or nullptr
  bool isIndirect = false;
  AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE;
};

extern const FieldEntry radio_fields[];
extern const FieldEntry active_bands_fields[];

// extern ResultCode lookup_field_path(
//   const char* path,
//   uint32_t* tags_out,
//   uint32_t max_tags,
//   bool* is_indirect_out,
//   AutoCompleteTrigger* trigger_out);

