#include "SettingsBase.h"
#include <util/MessageTagLookup.h>

#include "pb_common.h"



ResultCode
SettingsBase::resolveDottedPath(const char* dottedPath, SettingFieldPath& path)
{
  return lookup_field_path(dottedPath, path.data(), MAX_FIELD_PATH_LENGTH);
}

ResultCode
SettingsBase::updateField(pb_field_iter_t* iter, const SettingFieldVariant &value)
{
   void* target_ptr = iter->pData;

  if (PB_ATYPE(iter->type) == PB_ATYPE_POINTER) {
    // For pointer types, dereference first
    target_ptr = *(void**)iter->pData;
    if (!target_ptr) {
      return ResultCode::ERR_SETTING_POINTER_FIELD_NULL;
    }
  }

  // Handle different field types
  pb_type_t ltype = PB_LTYPE(iter->type);
  switch (ltype) {
  case PB_LTYPE_BOOL: {
    const bool* pBoolValue = etl::get_if<bool>(&value);
    if (pBoolValue == nullptr) {
      return ResultCode::ERR_SETTING_EXPECT_BOOL;
    }
    *static_cast<bool *>(target_ptr) = *pBoolValue;
    break;
  }
  case PB_LTYPE_VARINT:
  case PB_LTYPE_SVARINT:
    if (iter->data_size == sizeof(int32_t)) {
      const int32_t* pInt32Value = etl::get_if<int32_t>(&value);
      if (pInt32Value == nullptr) {
        return ResultCode::ERR_SETTING_EXPECT_INT32;
      }
      *static_cast<int32_t *>(target_ptr) = *pInt32Value;
    } else if (iter->data_size == sizeof(int64_t)) {
      const int64_t* pInt64Value = etl::get_if<int64_t>(&value);
      if (pInt64Value == nullptr) {
        return ResultCode::ERR_SETTING_EXPECT_INT64;
      }
      *static_cast<int64_t *>(target_ptr) = *pInt64Value;
    }
    break;

  case PB_LTYPE_UVARINT:
    if (iter->data_size == sizeof(uint32_t)) {
      const uint32_t* pUint32Value = etl::get_if<uint32_t>(&value);
      if (pUint32Value == nullptr) {
        return ResultCode::ERR_SETTING_EXPECT_UINT32;
      }
      *static_cast<uint32_t *>(target_ptr) = *pUint32Value;
    } else if (iter->data_size == sizeof(uint64_t)) {
      const uint64_t* pUint64Value = etl::get_if<uint64_t>(&value);
      if (pUint64Value == nullptr) {
        return ResultCode::ERR_SETTING_EXPECT_UINT64;
      }
      *static_cast<uint64_t *>(target_ptr) = *pUint64Value;
    }
    break;

  case PB_LTYPE_FIXED32: {
    const float* pFloatValue = etl::get_if<float>(&value);
    if (pFloatValue == nullptr) {
      return ResultCode::ERR_SETTING_EXPECT_FLOAT;
    }
    *static_cast<float *>(target_ptr) = *pFloatValue;
    break;
  }
  case PB_LTYPE_FIXED64: {
    const double* pDoubleValue = etl::get_if<double>(&value);
    if (pDoubleValue == nullptr) {
      return ResultCode::ERR_SETTING_EXPECT_DOUBLE;
    }
    *static_cast<double *>(target_ptr) = *pDoubleValue;
    break;
  }
  case PB_LTYPE_STRING: {
    const StringValue stringValue = getStringValue(value);

    if (stringValue.data == nullptr) {
      return ResultCode::ERR_SETTING_EXPECT_STRING;
    }
    if (stringValue.size > iter->data_size - 1) {
      return ResultCode::ERR_SETTING_STRING_TOO_LONG;
    }
    strncpy(static_cast<char *>(target_ptr), stringValue.data, stringValue.size);
    static_cast<char *>(target_ptr)[stringValue.size - 1] = '\0';
    break;
  }
  default:
    return ResultCode::ERR_SETTING_UNSUPPORTED_TYPE;
  }
  // set has_<blah>
  if (iter->pSize != nullptr) {
    *static_cast<bool *>(iter->pSize) = true;
  }
  return ResultCode::OK;
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

ResultCode
SettingsBase::update(const SettingFieldPath &path, const SettingFieldVariant &value)
{
  pb_field_iter_t iter;
  void* current_message = getMessage();
  const pb_msgdesc_t* current_desc = m_pDescriptor;

  uint32_t pathLength = path.size();
  // Array to store parent iterators for updating pSize later
  pb_field_iter_t ancestor_iters[pathLength];
  size_t parent_count = 0;

  // For ordinary values, traverse through all but the last field ID.
  // If the value is a Steppable message then include that in the traversal
  bool leafIsSteppable = path.back() > 1000;
  size_t messageCount = leafIsSteppable ? pathLength : pathLength - 1;
  for (size_t i = 0; i < messageCount; i++) {
    if (!pb_field_iter_begin(&iter, current_desc, current_message)) {
      return ResultCode::ERR_SETTING_EMPTY_MESSAGE;
    }
    uint32_t tag = path.at(i);
    // Find the field with the specified tag
    if (!pb_field_iter_find(&iter, tag)) {
      return ResultCode::ERR_SETTING_FIELD_NOT_FOUND;
    }

    // Verify this is a submessage type
    if (!PB_LTYPE_IS_SUBMSG(iter.type)) {
      return ResultCode::ERR_SETTING_PATH_TOO_LONG;
    }

    // Store this iterator as a parent for potential pSize update
    ancestor_iters[parent_count++] = iter;

    // Get pointer to the submessage
    if (PB_ATYPE(iter.type) == PB_ATYPE_POINTER) {
      // For pointer type, dereference to get the submessage
      void** ptr = (void**)iter.pData;
      if (!*ptr) {
        return ResultCode::ERR_SETTING_POINTER_FIELD_NULL;
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
      return ResultCode::ERR_SETTING_PATH_TOO_LONG;
    }
  }

  // Now handle the final field (the one to update)
  if (!pb_field_iter_begin(&iter, current_desc, current_message)) {
    return ResultCode::ERR_SETTING_EMPTY_MESSAGE;
  }

  if (!pb_field_iter_find(&iter, path[pathLength - 1])) {
    return ResultCode::ERR_SETTING_UNKNOWN_TAG;
  }

  ResultCode result = updateField(&iter, value);
  // If the update was successful then mark all ancestors as changed
  if (result == ResultCode::OK) {
    for (size_t i = 0; i < parent_count; i++) {
      if (ancestor_iters[i].pSize) {
        *static_cast<bool *>(ancestor_iters[i].pSize) = true;
      }
    }
  }

  return result;
}

SettingsBase::StringValue
SettingsBase::StringValueVisitor::operator()(const NameString& value) const
{
  return {value.c_str(), static_cast<pb_size_t>(value.size())};
}

SettingsBase::StringValue
SettingsBase::StringValueVisitor::operator()(const LabelString& value) const
{
  return {value.c_str(), static_cast<pb_size_t>(value.size())};
}

SettingsBase::StringValue
SettingsBase::getStringValue(const SettingFieldVariant& value)
{
  return etl::visit(StringValueVisitor{}, value);
}
