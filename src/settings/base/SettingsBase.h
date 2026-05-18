#pragma once

#include "ResultCode.h"
#include "RadioSettings.pb.h"
#include <etl/string.h>

#define MAX_DOTTED_STRING_LENGTH 64

using StringRef = etl::string_ext;

class SettingsBase
{
public:
  SettingsBase() =default;
  virtual ~SettingsBase() = default;

  SettingsBase(const SettingsBase &rhs) = default;

  SettingsBase &operator=(const SettingsBase &rhs) = default;
};
