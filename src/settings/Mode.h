#pragma once
#include "base/SettingsBase.h"

class Mode : public SettingsBase
{
public:
  enum Type
  {
    NONE = RadioSettings_ModeType_MODE_NONE,
    AMN = RadioSettings_ModeType_MODE_AMN,
    AMW = RadioSettings_ModeType_MODE_AMW,
    LSB = RadioSettings_ModeType_MODE_LSB,
    USB = RadioSettings_ModeType_MODE_USB,
    FMN = RadioSettings_ModeType_MODE_FMN,
    FMW = RadioSettings_ModeType_MODE_FMW,
    CWL = RadioSettings_ModeType_MODE_CWL,
    CWU = RadioSettings_ModeType_MODE_CWU
  };

  Mode(RadioSettings_ModePb& raw)
    : SettingsBase(&RadioSettings_ModePb_msg)
    , m_rawSettings(raw)
    , m_name{m_rawSettings.name, sizeof(m_rawSettings.name)}
    , m_label{m_rawSettings.label, sizeof(m_rawSettings.label)}
  {}

  StringRef& name() { return m_name; }
  StringRef& label() { return m_label; }

  [[nodiscard]] int32_t loCut() const { return m_rawSettings.lo_cut; }
  [[nodiscard]] int32_t hiCut() const { return m_rawSettings.hi_cut; }
  [[nodiscard]] int32_t offset() const { return m_rawSettings.offset; }

  [[nodiscard]] Type type() const { return static_cast<Type>(m_rawSettings.type); }

protected:
  RadioSettings_ModePb& m_rawSettings;
  StringRef m_name;
  StringRef m_label;
};
