#include "settings/model/path/resolveDottedString.h"
#include <CrossPlatformTypes.h>

ResultCode resolveDottedString(const char* dottedPath, const FieldEntry* tableRoot, FieldDescriptor& descriptor)
{
  const FieldEntry* current_table = tableRoot;
  int tag_count = 0;

  const char* p = dottedPath;
  char field_name[MAX_FIELD_NAME_LENGTH];

  FieldPath& path = descriptor.getPath();
  bool isIndirect = false;
  bool needsAutoComplete = false;

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
        if (entry->isIndirect) {
          isIndirect = true;
        }
        if (current_table == nullptr) {
          // We're at the leaf. Save the needsAutoComplete flag
          needsAutoComplete = entry->needsAutoComplete;
        }
        found = true;
        break;
      }
      entry++;
    }

    if (!found) {
      return ResultCode::ERR_SETTING_PATH_NOT_FOUND;
    }
    if (current_table == nullptr) {
      if (*p != '\0') {
        return ResultCode::ERR_SETTING_DOTTED_STRING_NOT_VALID;
      }
    }
  }
  descriptor.setIsIndirect(isIndirect);
  descriptor.setNeedsAutoComplete(needsAutoComplete);
  return tag_count > 0 ? ResultCode::OK : ResultCode::ERR_SETTING_PATH_NOT_FOUND;
}
