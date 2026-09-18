#pragma once

#include "ConfigBase.h"

namespace Config::FieldDescriptor
{
  struct Fields
  {
    FieldPathTags tags;
    bool isIndirect;
    bool needsAutoComplete;
  };
}