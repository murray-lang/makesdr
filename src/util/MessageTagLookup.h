#pragma once

#include <cstdint>
#include <cstring>

// Field name to tag mappings (organized by message type)
struct FieldEntry {
    const char* name;
    int32_t tag;
    const FieldEntry* submsg;  // Pointer to submessage field table, or nullptr
};

extern const FieldEntry radio_fields[];

extern bool lookup_field_path(const char* path, uint32_t* tags_out, uint32_t max_tags);

