#include "PbUtils.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "pb_common.h"


bool pb_update_value(pb_field_iter_t* iter, const void* new_value, size_t value_size)
{
  // Update the value based on the field type
  void* target_ptr = iter->pData;

  if (PB_ATYPE(iter->type) == PB_ATYPE_POINTER) {
    // For pointer types, dereference first
    target_ptr = *(void**)iter->pData;
    if (!target_ptr) {
      return false; // Null pointer
    }
  }

  // Handle different field types
  pb_type_t ltype = PB_LTYPE(iter->type);
  switch (ltype) {
  case PB_LTYPE_BOOL:
    if (value_size >= sizeof(bool)) {
      *(bool*)target_ptr = *(const bool*)new_value;
    }
    break;

  case PB_LTYPE_VARINT:
  case PB_LTYPE_SVARINT:
    if (iter->data_size == sizeof(int32_t) && value_size >= sizeof(int32_t)) {
      *(int32_t*)target_ptr = *(const int32_t*)new_value;
    }
    else if (iter->data_size == sizeof(int64_t) && value_size >= sizeof(int64_t)) {
      *(int64_t*)target_ptr = *(const int64_t*)new_value;
    }
    break;

  case PB_LTYPE_UVARINT:
    if (iter->data_size == sizeof(uint32_t) && value_size >= sizeof(uint32_t)) {
      *(uint32_t*)target_ptr = *(const uint32_t*)new_value;
    }
    else if (iter->data_size == sizeof(uint64_t) && value_size >= sizeof(uint64_t)) {
      *(uint64_t*)target_ptr = *(const uint64_t*)new_value;
    }
    break;

  case PB_LTYPE_FIXED32:
    if (value_size >= sizeof(uint32_t)) {
      *(uint32_t*)target_ptr = *(const uint32_t*)new_value;
    }
    break;

  case PB_LTYPE_FIXED64:
    if (value_size >= sizeof(uint64_t)) {
      *(uint64_t*)target_ptr = *(const uint64_t*)new_value;
    }
    break;

  case PB_LTYPE_STRING:
    // For strings, copy up to the max size
    if (iter->data_size > 0) {
      strncpy((char*)target_ptr, (const char*)new_value, iter->data_size - 1);
      ((char*)target_ptr)[iter->data_size - 1] = '\0';
    }
    break;

  case PB_LTYPE_BYTES:
  case PB_LTYPE_FIXED_LENGTH_BYTES:
    // For bytes, copy the data
    if (value_size <= iter->data_size) {
      memcpy(target_ptr, new_value, value_size);
      if (ltype == PB_LTYPE_BYTES) {
        // Update the size field for pb_bytes_array_t
        pb_bytes_array_t* bytes = (pb_bytes_array_t*)target_ptr;
        bytes->size = value_size;
      }
    }
    break;

  default:
    return false;
  }
  if (iter->pSize != NULL) {
    *(bool*)iter->pSize = true;
  }
  return true;
}

void pb_calc_steppable_float(float oldValue, float delta, float coarseStep, float fineStep, bool useFine, float* newValue)
{
  if (useFine) {
    *newValue = oldValue + delta*fineStep;
  } else {
    *newValue = oldValue + delta*coarseStep;
  }
}

void pb_calc_steppable_int64(int64_t oldValue, int64_t delta, int64_t coarseStep, int64_t fineStep, int64_t useFine, int64_t* newValue)
{
  if (useFine) {
    *newValue = oldValue + delta*fineStep;
  } else {
    *newValue = oldValue + delta*coarseStep;
  }
}

bool pb_update_steppable(pb_field_iter_t* msg_iter, const void* value_or_delta, size_t value_size)
{
  void* steppable_msg = msg_iter->pData;
  const pb_msgdesc_t* steppable_desc = msg_iter->submsg_desc;
  pb_field_iter_t value_iter;
  pb_field_iter_t other_iter;

  if (!pb_field_iter_begin(&value_iter, steppable_desc, steppable_msg)) {
    return false;
  }
  // The value_iter stays unchanged so that the value can be updated.
  // Get another one to step through the other fields
  pb_field_iter_begin(&other_iter, steppable_desc, steppable_msg);

  // Find the value field
  if (!pb_field_iter_find(&value_iter, 1)) {
    return false;
  }
  void* value = value_iter.pData;
  pb_type_t type = PB_LTYPE(value_iter.type);

  if (!pb_field_iter_find(&other_iter, 2)) {
    return false;
  }
  void* coarse_delta = other_iter.pData;

  if (!pb_field_iter_find(&other_iter, 3)) {
    return false;
  }
  void* fine_delta = other_iter.pData;

  if (!pb_field_iter_find(&other_iter, 4)) {
    return false;
  }
  void* use_fine = other_iter.pData;
  bool useFine = *(bool*)use_fine;

  if (type == PB_LTYPE_FIXED32) {
    float result;
    float delta = *(float*)value_or_delta;
    pb_calc_steppable_float(*(float*)value, delta, *(float*)coarse_delta, *(float*)fine_delta, useFine, &result);

    return pb_update_value(&value_iter, &result, sizeof(float));
  }
  if (type == PB_LTYPE_VARINT) {
    int64_t result;
    int64_t delta = *(int64_t*)value_or_delta;
    pb_calc_steppable_int64(*(int64_t*)value, delta, *(int64_t*)coarse_delta, *(int64_t*)fine_delta, useFine, &result);

    return pb_update_value(&value_iter, (int64_t*)&result, sizeof(int64_t));
  }
  return false;
}

/**
 * Traverse a nanopb structure based on a list of field IDs and update the final field value
 *
 * @param desc Message descriptor for the root message
 * @param message Pointer to the root message structure
 * @param field_ids Array of field tags to traverse (e.g., [1, 5, 3])
 * @param field_ids_count Number of field IDs in the array
 * @param new_value Pointer to the new value to set
 * @param value_size Size of the new value in bytes
 * @return true if successful, false if field not found or invalid path
 */
bool pb_traverse_and_update(const pb_msgdesc_t* desc,
                            void* message,
                            const uint32_t* field_ids,
                            size_t field_ids_count,
                            const void* new_value,
                            size_t value_size)
{
  if (!desc || !message || !field_ids || field_ids_count == 0 || !new_value) {
    return false;
  }

  pb_field_iter_t iter;
  void* current_message = message;
  const pb_msgdesc_t* current_desc = desc;

  // Array to store parent iterators for updating pSize later
  pb_field_iter_t parent_iters[field_ids_count];
  size_t parent_count = 0;

  // For ordinary values, traverse through all but the last field ID.
  // If the value is a Steppable message then include that in the traversal
  bool leafIsSteppable = field_ids[field_ids_count - 1] > 1000;
  size_t messageCount = leafIsSteppable ? field_ids_count : field_ids_count - 1;
  for (size_t i = 0; i < messageCount; i++) {
    if (!pb_field_iter_begin(&iter, current_desc, current_message)) {
      return false;
    }
    uint32_t tag = field_ids[i];
    // Find the field with the specified tag
    if (!pb_field_iter_find(&iter, tag)) {
      return false;
    }

    // Verify this is a submessage type
    if (!PB_LTYPE_IS_SUBMSG(iter.type)) {
      return false;
    }

    // Store this iterator as a parent for potential pSize update
    parent_iters[parent_count++] = iter;

    // Get pointer to the submessage
    if (PB_ATYPE(iter.type) == PB_ATYPE_POINTER) {
      // For pointer type, dereference to get the submessage
      void** ptr = (void**)iter.pData;
      if (!*ptr) {
        return false; // Null pointer
      }
      current_message = *ptr;
    }
    else {
      // For static type, pData points directly to the submessage
      current_message = iter.pData;
    }

    // Tags over 1000 identify steppable fields
    if (tag > 1000) {
      return pb_update_steppable(&iter, new_value, value_size);
    }

    current_desc = iter.submsg_desc;
    if (!current_desc) {
      return false;
    }
  }

  // Now handle the final field (the one to update)
  if (!pb_field_iter_begin(&iter, current_desc, current_message)) {
    return false;
  }

  if (!pb_field_iter_find(&iter, field_ids[field_ids_count - 1])) {
    return false;
  }

  bool result = pb_update_value(&iter, new_value, value_size);
  if (result) {
    for (size_t i = 0; i < parent_count; i++) {
      if (parent_iters[i].pSize) {
        *(bool*)(parent_iters[i].pSize) = true;
      }
    }
  }

  return result;

}
