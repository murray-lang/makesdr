#pragma once
#include <CrossPlatformTypes.h>

#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/message/StringRef.h>


class Mode
{
public:
  using Proto = makesdr_ModePb;

  enum Type
  {
    NONE = makesdr_ModeType_MODE_NONE,
    AMN = makesdr_ModeType_MODE_AMN,
    AMW = makesdr_ModeType_MODE_AMW,
    LSB = makesdr_ModeType_MODE_LSB,
    USB = makesdr_ModeType_MODE_USB,
    FMN = makesdr_ModeType_MODE_FMN,
    FMW = makesdr_ModeType_MODE_FMW,
    CWL = makesdr_ModeType_MODE_CWL,
    CWU = makesdr_ModeType_MODE_CWU
  };

  Mode(Proto& raw)
    : m_rawRef(raw)
    , m_name{raw.name, raw.name, sizeof(raw.name)}
    , m_label{raw.label, raw.label, sizeof(raw.label)}
  {}

  Mode(const Proto& raw) // Ugly, but allows me to use getters on a const Proto
  : m_rawRef(const_cast<Proto&>(raw))
  , m_name{raw.name, const_cast<char*>(raw.name), sizeof(raw.name)}
  , m_label{raw.label, const_cast<char*>(raw.label), sizeof(raw.label)}
  {}

  Mode(const Mode& rhs) noexcept
    : m_rawRef(rhs.m_rawRef)
    , m_name{rhs.m_rawRef.name, rhs.m_rawRef.name, sizeof(rhs.m_rawRef.name)}
  , m_label{rhs.m_rawRef.label, rhs.m_rawRef.label, sizeof(rhs.m_rawRef.label)}
  {
  }

  Mode(const Mode&& rhs) noexcept
    : m_rawRef(rhs.m_rawRef)
    , m_name{rhs.m_rawRef.name, rhs.m_rawRef.name, sizeof(rhs.m_rawRef.name)}
    , m_label{rhs.m_rawRef.label, rhs.m_rawRef.label, sizeof(rhs.m_rawRef.label)}
  {
  }

  Mode& operator=(const Mode& rhs) noexcept
  {
    return operator=(rhs.m_rawRef);
  }

  Mode& operator=(const Proto& rhs) noexcept
  {
    m_rawRef = rhs;
    m_name = m_rawRef.name;
    m_label = m_rawRef.label;
    return *this;
  }

  Proto& raw() { return m_rawRef; }
  [[nodiscard]] const Proto& raw() const { return m_rawRef; }

  [[nodiscard]] Type type() const { return static_cast<Type>(m_rawRef.type); }

  // StringRef& name() { return m_name; }
  [[nodiscard]] const StringRef& name() const { return m_name; }
  // StringRef& label() { return m_label; }
  [[nodiscard]] const StringRef& label() const { return m_label; }

  [[nodiscard]] int32_t loCut() const { return m_rawRef.lo_cut; }
  [[nodiscard]] int32_t hiCut() const { return m_rawRef.hi_cut; }
  [[nodiscard]] int32_t offset() const { return m_rawRef.offset; }

protected:
  Proto& m_rawRef;
  StringRef m_name;
  StringRef m_label;
};

using ModeOrRequestVariant = variant<monostate, Mode::Type, Mode>;
