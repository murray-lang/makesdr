#pragma once
#include "MessageVisitor.h"
#include "SettingsBase.h"

class BaseRadioSettings : public SettingsBase
{
public:
  BaseRadioSettings(RadioSettings_RadioSettingsPb& raw)
    : m_rawSettings(raw)
    , m_visitor(&raw, &RadioSettings_RadioSettingsPb_msg)
  {
  }

  ResultCode updateField(const SettingFieldUpdate &settingUpdate)
  {
    return m_visitor.updateField(settingUpdate);
  }

  ResultCode updateField(const SettingFieldPath &path, const SettingFieldVariant &value)
  {
    return m_visitor.updateField(path, value);
  }

  ResultCode getField(const SettingFieldPath &path, SettingFieldVariant &value)
  {
    return m_visitor.getField(path, value);
  }

  ResultCode getField(
    const SettingFieldPath &path,
    SettingFieldVariant &value,
    bool mustHave,
    bool parentsMustHave,
    bool& retrieved
  )
  {
    return m_visitor.getField(path, value, mustHave, parentsMustHave, retrieved);
  }

  ResultCode setFieldPresence(const SettingFieldPath &path, bool present)
  {
    return m_visitor.setFieldPresence(path, present);
  }

  ResultCode mergePresentFields(const void* pRhsMessage)
  {
    return m_visitor.mergePresentFields(pRhsMessage);
  }

  static ResultCode resolveDottedPath(
    const char *dottedPath,
    SettingFieldPath &path,
    bool* isIndirectOut,
    AutoCompleteTrigger* triggerOut
  )
  {
    return MessageVisitor::resolveDottedPath(dottedPath, path, isIndirectOut, triggerOut);
  }


protected:
  RadioSettings_RadioSettingsPb& m_rawSettings;

  MessageVisitor m_visitor;
};