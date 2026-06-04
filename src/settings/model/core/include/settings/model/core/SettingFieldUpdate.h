#pragma once
#include <CrossPlatformTypes.h>
#include "SettingFieldPath.h"
#include "SettingFieldVariant.h"
#include "AutoCompleteTrigger.h"

#define MAX_SETTING_UPDATE_SEQUENCE 4

class SettingFieldUpdate
{
public:

  enum Meaning { NONE = 0, VALUE, DELTA };

  SettingFieldUpdate(
    const SettingFieldPath& path,
    Meaning meaning,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_meaning(meaning)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}

  SettingFieldUpdate(
    const SettingFieldPath& path,
    const SettingFieldVariant& value,
    Meaning meaning,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(value)
    , m_meaning(meaning)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}
#ifdef USE_ETL
  SettingFieldUpdate(
    const SettingFieldPath& path,
    const NameString& value,
    Meaning meaning,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(in_place_type_t<NameString>{}, value)
    , m_meaning(meaning)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}

  SettingFieldUpdate(
    const SettingFieldPath& path,
    const LabelString& value,
    Meaning meaning,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(in_place_type_t<LabelString>{}, value)
    , m_meaning(meaning)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}
#else
  SettingFieldUpdate(
    const SettingFieldPath& path,
    const std::string& value,
    Meaning meaning,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(in_place_type_t<NameString>{}, value)
    , m_meaning(meaning)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}
#endif

  SettingFieldUpdate(
    const SettingFieldPath& path,
    int32_t value,
    Meaning meaning,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(value)
    , m_meaning(meaning)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}
  SettingFieldUpdate(
    const SettingFieldPath& path,
    uint32_t value,
    Meaning meaning,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(value)
    , m_meaning(meaning)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}
  SettingFieldUpdate(
    const SettingFieldPath& path,
    int64_t value,
    Meaning meaning,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(value)
    , m_meaning(meaning)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}
  SettingFieldUpdate(
    const SettingFieldPath& path,
    float value,
    Meaning meaning,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(value)
    , m_meaning(meaning)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}
  SettingFieldUpdate(
    const SettingFieldPath& path,
    bool value,
    Meaning meaning,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_value(value)
    , m_meaning(meaning)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}

  [[nodiscard]] const SettingFieldPath& path() const { return m_path; }
  [[nodiscard]] Meaning meaning() const { return m_meaning; }
  [[nodiscard]] const SettingFieldVariant& value() const { return m_value; }
  [[nodiscard]] bool isIndirect() const { return m_isIndirect; }
  [[nodiscard]] AutoCompleteTrigger trigger() const { return m_trigger; }

#ifdef USE_ETL
  void setValue(const NameString& value) { m_value = value; }
  void setValue(const LabelString& value) { m_value = value; }
#else
  void setValue(const std::string& value) { m_value = value; }
#endif
  void setValue(int32_t value) { m_value = value; }
  void setValue(uint32_t value) { m_value = value; }
  void setValue(int64_t value) { m_value = value; }
  void setValue(float value) { m_value = value; }
  void setValue(bool value) { m_value = value; }

protected:
  SettingFieldPath m_path;
  SettingFieldVariant m_value;
  Meaning m_meaning;
  bool m_isIndirect;
  AutoCompleteTrigger m_trigger;
};

using SettingFieldUpdateVector = etl::vector<SettingFieldUpdate, MAX_SETTING_UPDATE_SEQUENCE>;