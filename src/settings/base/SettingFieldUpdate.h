#pragma once
#include "SettingFieldPath.h"
#include "SettingFieldVariant.h"

#define MAX_SETTING_UPDATE_SEQUENCE 4

class SettingFieldUpdate
{
public:
  SettingFieldUpdate(const SettingFieldPath& path, const SettingFieldVariant& value)
    : m_path(path)
    , m_value(value)
  {}
  SettingFieldUpdate(const SettingFieldPath& path, const NameString& value)
    : m_path(path)
    , m_value(etl::in_place_type_t<NameString>{}, value)
  {}
  SettingFieldUpdate(const SettingFieldPath& path, const LabelString& value)
    : m_path(path)
    , m_value(etl::in_place_type_t<LabelString>{}, value)
  {}
  SettingFieldUpdate(const SettingFieldPath& path, int32_t value)
    : m_path(path)
    , m_value(value)
  {}
  SettingFieldUpdate(const SettingFieldPath& path, uint32_t value)
    : m_path(path)
    , m_value(value)
  {}
  SettingFieldUpdate(const SettingFieldPath& path, int64_t value)
    : m_path(path)
    , m_value(value)
  {}
  SettingFieldUpdate(const SettingFieldPath& path, float value)
    : m_path(path)
    , m_value(value)
  {}
  SettingFieldUpdate(const SettingFieldPath& path, bool value)
    : m_path(path)
    , m_value(value)
  {}

  [[nodiscard]] const SettingFieldPath& path() const { return m_path; }
  [[nodiscard]] const SettingFieldVariant& value() const { return m_value; }

protected:
  SettingFieldPath m_path;
  SettingFieldVariant m_value;
};

using SettingFieldUpdateVector = etl::vector<SettingFieldUpdate, MAX_SETTING_UPDATE_SEQUENCE>;