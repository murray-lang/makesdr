#pragma once
#include "SettingFieldPath.h"
#include "SettingFieldVariant.h"
#include "writeable/AutoCompleteTrigger.h"

#define MAX_SETTING_UPDATE_SEQUENCE 4

class SettingFieldUpdate
{
public:
  SettingFieldUpdate(
    const SettingFieldPath& path,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}

  SettingFieldUpdate(
    const SettingFieldPath& path,
    const SettingFieldVariant& value,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(value)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}

  SettingFieldUpdate(
    const SettingFieldPath& path,
    const NameString& value,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(etl::in_place_type_t<NameString>{}, value)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}

  SettingFieldUpdate(
    const SettingFieldPath& path,
    const LabelString& value,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(etl::in_place_type_t<LabelString>{}, value)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}
  SettingFieldUpdate(
    const SettingFieldPath& path,
    int32_t value,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(value)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}
  SettingFieldUpdate(
    const SettingFieldPath& path,
    uint32_t value,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(value)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}
  SettingFieldUpdate(
    const SettingFieldPath& path,
    int64_t value,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(value)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}
  SettingFieldUpdate(
    const SettingFieldPath& path,
    float value,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(value)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}
  SettingFieldUpdate(
    const SettingFieldPath& path,
    bool value,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(value)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}

  [[nodiscard]] const SettingFieldPath& path() const { return m_path; }
  [[nodiscard]] const SettingFieldVariant& value() const { return m_value; }
  [[nodiscard]] bool isIndirect() const { return m_isIndirect; }
  [[nodiscard]] AutoCompleteTrigger trigger() const { return m_trigger; }

  void setValue(const NameString& value) { m_value = value; }
  void setValue(const LabelString& value) { m_value = value; }
  void setValue(int32_t value) { m_value = value; }
  void setValue(uint32_t value) { m_value = value; }
  void setValue(int64_t value) { m_value = value; }
  void setValue(float value) { m_value = value; }
  void setValue(bool value) { m_value = value; }

protected:
  SettingFieldPath m_path;
  SettingFieldVariant m_value;
  bool m_isIndirect;
  AutoCompleteTrigger m_trigger;
};

using SettingFieldUpdateVector = etl::vector<SettingFieldUpdate, MAX_SETTING_UPDATE_SEQUENCE>;