#pragma once
#include "pb.h"
#include "pb_common.h"
#include <vector>
#include <string>

#include "RadioSettings.pb.h"

#define GET_FIELD_ID(a, alloc, rule, type, name, tag) (tag)

#define MAX_MESSAGE_DEPTH 10

extern bool pb_traverse_and_update(const pb_msgdesc_t *desc,
                                   void *message,
                                   const uint32_t *field_ids,
                                   size_t field_ids_count,
                                   const void *new_value,
                                   size_t value_size);

