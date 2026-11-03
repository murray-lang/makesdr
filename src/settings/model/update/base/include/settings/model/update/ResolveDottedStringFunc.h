#pragma once

#include <ResultCode.h>
#include "FieldDescriptor.h"

#ifdef USE_ETL
#include <etl/inplace_function.h>
using ResolveDottedStringFunc = etl::inplace_function<ResultCode(const char*, FieldDescriptor&)>;
#else
#include <functional>
using ResolveDottedStringFunc = std::function<ResultCode(const char*, FieldDescriptor&)>;
#endif
