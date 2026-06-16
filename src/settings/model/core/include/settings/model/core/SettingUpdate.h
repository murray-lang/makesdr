#pragma once
#include <CrossPlatformTypes.h>
#include "SettingPath.h"
#include "SettingUpdateVariant.h"
#include "AutoCompleteTrigger.h"

#define MAX_SETTING_UPDATE_SEQUENCE 4

class SettingUpdate
{
public:

  enum Meaning { NONE = 0, VALUE, DELTA };

  SettingUpdate(
    const SettingPath& path,
    Meaning meaning,
    bool isIndirect = false,
    const AutoCompleteTrigger trigger = AutoCompleteTrigger::NONE
    )
    : m_path(path)
    , m_meaning(meaning)
    , m_isIndirect(isIndirect)
    , m_trigger(trigger)
  {}

  SettingUpdate(
    const SettingPath& path,
    const SettingUpdateVariant& value,
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
  SettingUpdate(
    const SettingPath& path,
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

  SettingUpdate(
    const SettingPath& path,
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
  SettingUpdate(
    const SettingPath& path,
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

  SettingUpdate(
    const SettingPath& path,
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
  SettingUpdate(
    const SettingPath& path,
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
  SettingUpdate(
    const SettingPath& path,
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
  SettingUpdate(
    const SettingPath& path,
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
  SettingUpdate(
    const SettingPath& path,
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

  [[nodiscard]] const SettingPath& path() const { return m_path; }
  [[nodiscard]] Meaning meaning() const { return m_meaning; }
  [[nodiscard]] const SettingUpdateVariant& value() const { return m_value; }
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
  SettingPath m_path;
  SettingUpdateVariant m_value;
  Meaning m_meaning;
  bool m_isIndirect;
  AutoCompleteTrigger m_trigger;
};

using SettingUpdateVector = etl::vector<SettingUpdate, MAX_SETTING_UPDATE_SEQUENCE>;