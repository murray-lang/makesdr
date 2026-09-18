#pragma once

#include <CrossPlatformTypes.h>
#include "FieldDescriptor.h"

using ResolveDottedStringFunc = function<ResultCode(const char *, FieldDescriptor& )>;
