#pragma once

#include "RadioSettings.pb.h"
#include "SettingFieldPath.h"
#include <etl/string.h>
#include <ResultCode.h>
#include "SettingFieldUpdate.h"

#define MAX_DOTTED_STRING_LENGTH 64

using StringRef = etl::string_ext;

class SettingsBase
{
public:
  SettingsBase(const pb_msgdesc_t *pDescriptor)
    : m_pDescriptor(pDescriptor)
  {
  }

  SettingsBase(const SettingsBase &rhs) = default;

  virtual ~SettingsBase() = default;

  SettingsBase &operator=(const SettingsBase &rhs) = default;

  ResultCode updateField(const SettingFieldUpdate &settingUpdate)
  {
    return updateField(settingUpdate.path(), settingUpdate.value());
  }

  ResultCode updateField(const SettingFieldPath &path, const SettingFieldVariant &value);

  ResultCode getField(const SettingFieldPath &path, SettingFieldVariant &value);

  static ResultCode resolveDottedPath(const char *dottedPath, SettingFieldPath &path);

protected:
  ResultCode updateField(pb_field_iter_t* iter, const SettingFieldVariant& value);
  ResultCode updateSteppable(pb_field_iter_t* iter, const SettingFieldVariant& value);
  ResultCode getField(pb_field_iter_t* iter, SettingFieldVariant& value);

  virtual void* getMessage() = 0;

private:
  struct StringValue
  {
    const char* data = nullptr;
    pb_size_t size = 0;
  };

  struct StringValueVisitor
  {
    StringValue operator()(const NameString& value) const;
    StringValue operator()(const LabelString& value) const;

    template <typename T>
    StringValue operator()(const T&) const
    {
      return {};
    }
  };

  static StringValue getStringValue(const SettingFieldVariant& value);

protected:
  const pb_msgdesc_t *m_pDescriptor;
};
