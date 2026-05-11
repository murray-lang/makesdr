#pragma once
#include "SettingFieldPath.h"
#include "SettingFieldVariant.h"

class SettingFieldUpdate
{
public:
  SettingFieldUpdate(const SettingFieldPath& path, const SettingFieldVariant& value)
    : m_path(path)
    , m_value(value)
  {}
  SettingFieldUpdate(const SettingFieldPath& path, const NameString& value)
    : m_path(path)
    , m_value(value)
  {}
  SettingFieldUpdate(const SettingFieldPath& path, const LabelString& value)
    : m_path(path)
    , m_value(value)
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

  const SettingFieldPath& path() const { return m_path; }
  const SettingFieldVariant& value() const { return m_value; }

protected:
  SettingFieldPath m_path;
  SettingFieldVariant m_value;
};