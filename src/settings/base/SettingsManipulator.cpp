#include "SettingsManipulator.h"
#include <util/MessageTagLookup.h>

#include "pb_common.h"
#include <cstring>


ResultCode
SettingsManipulator::resolveDottedPath(const char* dottedPath, SettingFieldPath& path)
{
  //return lookup_field_path(dottedPath, path.data(), MAX_FIELD_PATH_LENGTH);

  const FieldEntry* current_table = radio_fields;
  int tag_count = 0;

  const char* p = dottedPath;
  char field_name[MAX_FIELD_NAME_LENGTH];

  while (*p && tag_count < MAX_FIELD_PATH_LENGTH) {
    // Extract next field name
    int i = 0;
    while (*p && *p != '.' && i < 63) {
      field_name[i++] = *p++;
    }
    field_name[i] = '\0';
    if (*p == '.') p++;  // Skip dot

    // Search in current table
    const FieldEntry* entry = current_table;
    bool found = false;
    while (entry != nullptr && entry->name != nullptr) {
      if (strcmp(entry->name, field_name) == 0) {
        path.push_back(entry->tag);
        tag_count++;
        current_table = entry->submsg;
        found = true;
        break;
      }
      entry++;
    }

    if (!found) {
      return ResultCode::ERR_SETTING_PATH_NOT_FOUND;
    }
    if (current_table == nullptr && *p != '\0') {
      return ResultCode::ERR_SETTING_DOTTED_STRING_NOT_VALID;
    }
  }

  return tag_count > 0 ? ResultCode::OK : ResultCode::ERR_SETTING_PATH_NOT_FOUND;
}

ResultCode
SettingsManipulator::updateField(pb_field_iter_t* iter, const SettingFieldVariant &value)
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
    static_cast<char *>(target_ptr)[stringValue.size] = '\0';
    break;
  }
  default:
    return ResultCode::ERR_SETTING_UNSUPPORTED_TYPE;
  }
  // set has_<blah> or which_<blah>
  markFieldPresent(iter);
  return ResultCode::OK;
}

void
SettingsManipulator::pb_calc_steppable_float(float oldValue, float delta, float coarseStep, float fineStep, bool useFine, float* newValue)
{
  if (useFine) {
    *newValue = oldValue + delta*fineStep;
  } else {
    *newValue = oldValue + delta*coarseStep;
  }
}

void
SettingsManipulator::pb_calc_steppable_int64(int64_t oldValue, int64_t delta, int64_t coarseStep, int64_t fineStep, int64_t useFine, int64_t* newValue)
{
  if (useFine) {
    *newValue = oldValue + delta*fineStep;
  } else {
    *newValue = oldValue + delta*coarseStep;
  }
}

ResultCode
SettingsManipulator::updateSteppable(pb_field_iter_t* msg_iter, const SettingFieldVariant& steppableValue)
{
  void* steppable_msg = msg_iter->pData;
  const pb_msgdesc_t* steppable_desc = msg_iter->submsg_desc;
  pb_field_iter_t value_iter;
  pb_field_iter_t other_iter;

  if (!pb_field_iter_begin(&value_iter, steppable_desc, steppable_msg)) {
    return ResultCode::ERR_SETTING_EMPTY_MESSAGE;
  }
  // The value_iter stays unchanged so that the value can be updated.
  // Get another one to step through the other fields
  pb_field_iter_begin(&other_iter, steppable_desc, steppable_msg);

  // Find the value field
  if (!pb_field_iter_find(&value_iter, 1)) {
    return ResultCode::ERR_SETTING_FIELD_NOT_FOUND;
  }
  void* value = value_iter.pData;
  pb_type_t type = PB_LTYPE(value_iter.type);

  if (!pb_field_iter_find(&other_iter, 2)) { // "Coarse delta tag
    return ResultCode::ERR_SETTING_STEPPABLE_COARSE_NOT_FOUND;
  }
  void* coarse_delta = other_iter.pData;

  if (!pb_field_iter_find(&other_iter, 3)) {
    return ResultCode::ERR_SETTING_STEPPABLE_FINE_NOT_FOUND;
  }
  void* fine_delta = other_iter.pData;

  if (!pb_field_iter_find(&other_iter, 4)) {
    return ResultCode::ERR_SETTING_STEPPABLE_USE_FINE_NOT_FOUND;
  }
  void* use_fine = other_iter.pData;
  bool useFine = *(bool*)use_fine;

  if (type == PB_LTYPE_FIXED32) {
    const float* pDelta = etl::get_if<float>(&steppableValue);
    if (pDelta == nullptr) {
      return ResultCode::ERR_SETTING_EXPECT_FLOAT;
    }
    float result;
    pb_calc_steppable_float(*(float*)value, *pDelta, *(float*)coarse_delta, *(float*)fine_delta, useFine, &result);

    return updateField(&value_iter, result);
  }
  if (type == PB_LTYPE_VARINT) {
    const int64_t* pDelta = etl::get_if<int64_t>(&steppableValue);
    if (pDelta == nullptr) {
      return ResultCode::ERR_SETTING_EXPECT_INT64;
    }
    int64_t result;
    pb_calc_steppable_int64(*(int64_t*)value, *pDelta, *(int64_t*)coarse_delta, *(int64_t*)fine_delta, useFine, &result);

    return updateField(&value_iter, result);
  }
  return ResultCode::ERR_SETTING_STEPPABLE_TYPE_UNSUPPORTED;
}

ResultCode
SettingsManipulator::updateField(const SettingFieldPath &path, const SettingFieldVariant &value)
{
  pb_field_iter_t iter;
  void* current_message = m_pMessage;
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
      ResultCode rc = updateSteppable(&iter, value);
      if (rc == ResultCode::OK) {
        for (size_t i = 0; i < parent_count; i++) {
          if (ancestor_iters[i].pSize) {
            *static_cast<bool *>(ancestor_iters[i].pSize) = true;
          }
        }
      }
      return rc;
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

ResultCode
SettingsManipulator::markFieldPresent(const pb_field_iter_t* iter)
{
  if (iter == nullptr || iter->pSize == nullptr) {
    return ResultCode::ERR_SETTING_EXPECT_OPTIONAL_OR_ONEOF;
  }

  if (PB_HTYPE(iter->type) == PB_HTYPE_ONEOF) {
    *static_cast<pb_size_t*>(iter->pSize) = iter->tag;
  } else {
    *static_cast<bool*>(iter->pSize) = true;
  }
  return ResultCode::OK;
}

ResultCode
SettingsManipulator::mergePresentFields(const void* pRhsMessage)
{
  if (m_pMessage == nullptr || pRhsMessage == nullptr || m_pDescriptor == nullptr) {
    return ResultCode::ERR_SETTING_POINTER_FIELD_NULL;
  }

  return mergePresentFields(m_pMessage, pRhsMessage, m_pDescriptor);
}

bool
SettingsManipulator::fieldHasMergeablePresence(const pb_field_iter_t* iter)
{
  if (iter == nullptr) {
    return false;
  }

  const pb_type_t htype = PB_HTYPE(iter->type);

  if (htype == PB_HTYPE_OPTIONAL) {
    return iter->pSize != nullptr && *static_cast<const bool*>(iter->pSize);
  }

  if (htype == PB_HTYPE_REPEATED) {
    return iter->pSize != nullptr && *static_cast<const pb_size_t*>(iter->pSize) > 0;
  }

  if (htype == PB_HTYPE_ONEOF) {
    if (iter->pSize == nullptr) {
      return false;
    }

    const pb_size_t which = *static_cast<const pb_size_t*>(iter->pSize);

    /*
     * Patch semantics:
     *   which_* == 0       -> rhs did not update this oneof
     *   which_* == tag     -> rhs selected this oneof member
     *   which_* != tag     -> another oneof member is selected; ignore this member
     */
    return which != 0 && which == iter->tag;
  }

  return false;
}

bool
SettingsManipulator::shouldVisitField(const pb_field_iter_t* iter)
{
  if (iter == nullptr) {
    return false;
  }

  if (fieldHasMergeablePresence(iter)) {
    return true;
  }

  /*
   * Singular submessages do not have has_* presence, but they may contain
   * optional/oneof fields deeper in the tree. Visit them as structural
   * containers. Singular scalar fields are skipped because there is no
   * reliable presence signal in a zero-init patch message.
   */
  return PB_LTYPE_IS_SUBMSG(iter->type) &&
         PB_HTYPE(iter->type) != PB_HTYPE_OPTIONAL &&
         PB_HTYPE(iter->type) != PB_HTYPE_REPEATED &&
         PB_HTYPE(iter->type) != PB_HTYPE_ONEOF;
}

ResultCode
SettingsManipulator::copyPresentField(pb_field_iter_t* pLhsIter, const pb_field_iter_t* pRhsIter)
{
  if (pLhsIter == nullptr || pRhsIter == nullptr) {
    return ResultCode::ERR_SETTING_POINTER_FIELD_NULL;
  }

  if (PB_ATYPE(pLhsIter->type) == PB_ATYPE_POINTER ||
      PB_ATYPE(pRhsIter->type) == PB_ATYPE_POINTER) {
    return ResultCode::ERR_SETTING_UNSUPPORTED_TYPE;
  }

  if (PB_ATYPE(pLhsIter->type) == PB_ATYPE_CALLBACK ||
      PB_ATYPE(pRhsIter->type) == PB_ATYPE_CALLBACK) {
    return ResultCode::ERR_SETTING_UNSUPPORTED_TYPE;
  }

  const pb_type_t htype = PB_HTYPE(pRhsIter->type);

  if (htype == PB_HTYPE_ONEOF) {
    if (pLhsIter->pSize == nullptr || pRhsIter->pSize == nullptr) {
      return ResultCode::ERR_SETTING_EXPECT_OPTIONAL_OR_ONEOF;
    }

    const pb_size_t rhsWhich = *static_cast<const pb_size_t*>(pRhsIter->pSize);

    if (rhsWhich == 0 || rhsWhich != pRhsIter->tag) {
      return ResultCode::OK;
    }

    std::memcpy(pLhsIter->pData, pRhsIter->pData, pRhsIter->data_size);
    *static_cast<pb_size_t*>(pLhsIter->pSize) = rhsWhich;
    return ResultCode::OK;
  }

  if (htype == PB_HTYPE_REPEATED) {
    if (pLhsIter->pSize == nullptr || pRhsIter->pSize == nullptr) {
      return ResultCode::ERR_SETTING_EXPECT_OPTIONAL_OR_ONEOF;
    }

    const pb_size_t rhsCount = *static_cast<const pb_size_t*>(pRhsIter->pSize);

    if (rhsCount == 0) {
      return ResultCode::OK;
    }

    std::memcpy(pLhsIter->pData, pRhsIter->pData, pRhsIter->data_size * rhsCount);
    *static_cast<pb_size_t*>(pLhsIter->pSize) = rhsCount;
    return ResultCode::OK;
  }

  std::memcpy(pLhsIter->pData, pRhsIter->pData, pRhsIter->data_size);

  if (htype == PB_HTYPE_OPTIONAL) {
    return markFieldPresent(pLhsIter);
  }

  return ResultCode::OK;
}

ResultCode
SettingsManipulator::mergePresentFields(
  void* pLhsMessage,
  const void* pRhsMessage,
  const pb_msgdesc_t* pDescriptor
)
{
  if (pLhsMessage == nullptr || pRhsMessage == nullptr || pDescriptor == nullptr) {
    return ResultCode::ERR_SETTING_POINTER_FIELD_NULL;
  }

  pb_field_iter_t lhsIter;
  pb_field_iter_t rhsIter;

  const bool lhsHasField = pb_field_iter_begin(&lhsIter, pDescriptor, pLhsMessage);
  const bool rhsHasField = pb_field_iter_begin(&rhsIter, pDescriptor, const_cast<void*>(pRhsMessage));

  if (!lhsHasField || !rhsHasField) {
    return lhsHasField == rhsHasField ? ResultCode::OK : ResultCode::ERR_SETTING_FIELD_NOT_FOUND;
  }

  bool lhsValid = true;
  bool rhsValid = true;

  while (lhsValid && rhsValid) {
    if (lhsIter.tag != rhsIter.tag) {
      return ResultCode::ERR_SETTING_FIELD_NOT_FOUND;
    }

    if (shouldVisitField(&rhsIter)) {
      if (PB_ATYPE(lhsIter.type) == PB_ATYPE_POINTER ||
          PB_ATYPE(rhsIter.type) == PB_ATYPE_POINTER) {
        return ResultCode::ERR_SETTING_UNSUPPORTED_TYPE;
      }

      if (PB_ATYPE(lhsIter.type) == PB_ATYPE_CALLBACK ||
          PB_ATYPE(rhsIter.type) == PB_ATYPE_CALLBACK) {
        return ResultCode::ERR_SETTING_UNSUPPORTED_TYPE;
      }

      if (PB_LTYPE_IS_SUBMSG(rhsIter.type)) {
        if (rhsIter.submsg_desc == nullptr) {
          return ResultCode::ERR_SETTING_PATH_TOO_LONG;
        }

        const pb_type_t rhsHtype = PB_HTYPE(rhsIter.type);

        if (rhsHtype == PB_HTYPE_ONEOF) {
          if (lhsIter.pSize == nullptr || rhsIter.pSize == nullptr) {
            return ResultCode::ERR_SETTING_EXPECT_OPTIONAL_OR_ONEOF;
          }

          const pb_size_t rhsWhich = *static_cast<const pb_size_t*>(rhsIter.pSize);

          /*
           * rhs.which_* == 0 means "ignore this oneof".
           * Normally this cannot be reached because shouldVisitField() filters
           * it out, but keep the guard here to preserve patch semantics.
           */
          if (rhsWhich == 0 || rhsWhich != rhsIter.tag) {
            lhsValid = pb_field_iter_next(&lhsIter);
            rhsValid = pb_field_iter_next(&rhsIter);
            continue;
          }

          const pb_size_t lhsWhich = *static_cast<const pb_size_t*>(lhsIter.pSize);

          /*
           * rhs selected a oneof member. That selected member has precedence.
           * If lhs had a different active member, clear this member's storage
           * before merging into it.
           */
          if (lhsWhich != rhsWhich) {
            std::memset(lhsIter.pData, 0, lhsIter.data_size);
          }

          *static_cast<pb_size_t*>(lhsIter.pSize) = rhsWhich;
        }

        ResultCode rc = mergePresentFields(lhsIter.pData, rhsIter.pData, rhsIter.submsg_desc);
        if (rc != ResultCode::OK) {
          return rc;
        }

        if (PB_HTYPE(lhsIter.type) == PB_HTYPE_OPTIONAL) {
          rc = markFieldPresent(&lhsIter);
          if (rc != ResultCode::OK) {
            return rc;
          }
        }
      } else {
        ResultCode rc = copyPresentField(&lhsIter, &rhsIter);
        if (rc != ResultCode::OK) {
          return rc;
        }
      }
    }

    lhsValid = pb_field_iter_next(&lhsIter);
    rhsValid = pb_field_iter_next(&rhsIter);
  }

  return lhsValid == rhsValid ? ResultCode::OK : ResultCode::ERR_SETTING_FIELD_NOT_FOUND;
}

ResultCode
SettingsManipulator::getField(pb_field_iter_t* iter, SettingFieldVariant& value)
{
  void* source_ptr = iter->pData;

  if (PB_ATYPE(iter->type) == PB_ATYPE_POINTER) {
    // For pointer types, dereference first
    source_ptr = *(void**)iter->pData;
    if (!source_ptr) {
      return ResultCode::ERR_SETTING_POINTER_FIELD_NULL;
    }
  }

  // Handle different field types
  pb_type_t ltype = PB_LTYPE(iter->type);
  switch (ltype) {
  case PB_LTYPE_BOOL:
    value = *static_cast<bool*>(source_ptr);
    break;

  case PB_LTYPE_VARINT:
  case PB_LTYPE_SVARINT:
    if (iter->data_size == sizeof(int32_t)) {
      value = *static_cast<int32_t*>(source_ptr);
    } else if (iter->data_size == sizeof(int64_t)) {
      value = *static_cast<int64_t*>(source_ptr);
    }
    break;

  case PB_LTYPE_UVARINT:
    if (iter->data_size == sizeof(uint32_t)) {
      value = *static_cast<uint32_t*>(source_ptr);
    } else if (iter->data_size == sizeof(uint64_t)) {
      value = *static_cast<int64_t*>(source_ptr); // Note: converting to signed (limited number of types in variant)
    }
    break;

  case PB_LTYPE_FIXED32:
    value = *static_cast<float*>(source_ptr);
    break;

  // case PB_LTYPE_FIXED64:
  //   value = *static_cast<double*>(source_ptr);
  //   break;

  case PB_LTYPE_STRING: {
    const char* str = static_cast<const char*>(source_ptr);
    // Use NameString as the default string type for retrieval
    value = NameString(str);
    break;
  }

  default:
    return ResultCode::ERR_SETTING_UNSUPPORTED_TYPE;
  }

  return ResultCode::OK;
}

ResultCode
SettingsManipulator::getField(const SettingFieldPath &path, SettingFieldVariant &value)
{
  pb_field_iter_t iter;
  void* current_message = m_pMessage;
  const pb_msgdesc_t* current_desc = m_pDescriptor;

  uint32_t pathLength = path.size();

  // Traverse through all but the last field ID
  for (size_t i = 0; i < pathLength - 1; i++) {
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

    current_desc = iter.submsg_desc;
    if (!current_desc) {
      return ResultCode::ERR_SETTING_PATH_TOO_LONG;
    }
  }

  // Now handle the final field (the one to retrieve)
  if (!pb_field_iter_begin(&iter, current_desc, current_message)) {
    return ResultCode::ERR_SETTING_EMPTY_MESSAGE;
  }

  if (!pb_field_iter_find(&iter, path[pathLength - 1])) {
    return ResultCode::ERR_SETTING_UNKNOWN_TAG;
  }

  return getField(&iter, value);
}

SettingsManipulator::StringValue
SettingsManipulator::StringValueVisitor::operator()(const NameString& value) const
{
  return {value.c_str(), static_cast<pb_size_t>(value.size())};
}

SettingsManipulator::StringValue
SettingsManipulator::StringValueVisitor::operator()(const LabelString& value) const
{
  return {value.c_str(), static_cast<pb_size_t>(value.size())};
}

SettingsManipulator::StringValue
SettingsManipulator::getStringValue(const SettingFieldVariant& value)
{
  return etl::visit(StringValueVisitor{}, value);
}



