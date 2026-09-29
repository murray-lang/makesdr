#pragma once

#include <ResultCode.h>
#include <settings/model/update/FieldEntry.h>
#include <settings/model/update/FieldDescriptor.h>

extern ResultCode resolveDottedString(const char *dottedPath, const FieldEntry* tableRoot, FieldDescriptor& descriptor);
