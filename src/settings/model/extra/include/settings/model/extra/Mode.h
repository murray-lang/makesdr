#pragma once
#include "settings/model/core/SettingsBase.h"
#include "settings/model/proto/RadioSettings.pb.h"
#include "StringRef.h"

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
    : m_rawSettings(raw)
    , m_name{raw.name, raw.name, sizeof(raw.name)}
    , m_label{raw.label, raw.label, sizeof(raw.label)}
  {}

  Mode(const Mode& rhs) noexcept
    : m_rawSettings(rhs.m_rawSettings)
    , m_name{rhs.m_rawSettings.name, rhs.m_rawSettings.name, sizeof(rhs.m_rawSettings.name)}
  , m_label{rhs.m_rawSettings.label, rhs.m_rawSettings.label, sizeof(rhs.m_rawSettings.label)}
  {
  }

  Mode(const Mode&& rhs) noexcept
    : m_rawSettings(rhs.m_rawSettings)
    , m_name{rhs.m_rawSettings.name, rhs.m_rawSettings.name, sizeof(rhs.m_rawSettings.name)}
    , m_label{rhs.m_rawSettings.label, rhs.m_rawSettings.label, sizeof(rhs.m_rawSettings.label)}
  {
  }

  RadioSettings_ModePb& raw() { return m_rawSettings; }
  const RadioSettings_ModePb& raw() const { return m_rawSettings; }

  [[nodiscard]] Type type() const { return static_cast<Type>(m_rawSettings.type); }

  StringRef& name() { return m_name; }
  StringRef& label() { return m_label; }

  [[nodiscard]] int32_t loCut() const { return m_rawSettings.lo_cut; }
  [[nodiscard]] int32_t hiCut() const { return m_rawSettings.hi_cut; }
  [[nodiscard]] int32_t offset() const { return m_rawSettings.offset; }

protected:
  RadioSettings_ModePb& m_rawSettings;
  StringRef m_name;
  StringRef m_label;
};
