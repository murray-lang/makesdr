#pragma once

#include <ResultCode.h>
#include <settings/model/path/FieldEntry.h>
#include <settings/model/message/FieldDescriptor.h>

extern ResultCode resolveDottedString(const char *dottedPath, const FieldEntry* tableRoot, FieldDescriptor& descriptor);
