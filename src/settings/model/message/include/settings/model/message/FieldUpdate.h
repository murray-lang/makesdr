#pragma once
#include <CrossPlatformTypes.h>

#include <utility>
#include "FieldDescriptor.h"
#include "FieldUpdateVariant.h"

#include <settings/model/proto/RadioSettings.pb.h>

#include "FieldUpdateMeaning.h"

#define MAX_SETTING_UPDATE_SEQUENCE 4

class FieldUpdateMessage;

class FieldUpdate
{
public:


  FieldUpdate(const FieldUpdateMessage& message);

  FieldUpdate(const FieldUpdate& rhs) noexcept = default;

  FieldUpdate(FieldDescriptor descriptor, FieldUpdateMeaning meaning, bool isFinal = true)
    : m_descriptor(::move(descriptor))
    , m_meaning(meaning)
    , m_isFinal(isFinal)
  {}

  FieldUpdate(FieldDescriptor descriptor, FieldUpdateVariant  value, FieldUpdateMeaning meaning, bool isFinal = true )
    : m_descriptor(::move(descriptor))
    , m_value(::move(value))
    , m_meaning(meaning)
    , m_isFinal(isFinal)
  {}
#ifdef USE_ETL
  FieldUpdate(FieldDescriptor descriptor, const NameString& value, Meaning meaning, bool isFinal = true)
    : m_descriptor(::move(descriptor))
    , m_value(in_place_type_t<NameString>{}, value)
    , m_meaning(meaning)
    , m_isFinal(isFinal)
  {}

  FieldUpdate(FieldDescriptor descriptor, const LabelString& value, Meaning meaning, bool isFinal = true)
    : m_descriptor(::move(descriptor))
    , m_value(in_place_type_t<LabelString>{}, value)
    , m_meaning(meaning)
    , m_isFinal(isFinal)
  {}
#else
  FieldUpdate(FieldDescriptor descriptor, const std::string& value, FieldUpdateMeaning meaning, bool isFinal = true)
    : m_descriptor(::move(descriptor))
    , m_value(in_place_type_t<NameString>{}, value)
    , m_meaning(meaning)
    , m_isFinal(isFinal)
  {}
#endif

  FieldUpdate(FieldDescriptor descriptor, int32_t value, FieldUpdateMeaning meaning, bool isFinal = true)
    : m_descriptor(::move(descriptor))
    , m_value(value)
    , m_meaning(meaning)
    , m_isFinal(isFinal)
  {}
  FieldUpdate(FieldDescriptor descriptor, uint32_t value, FieldUpdateMeaning meaning, bool isFinal = true)
    : m_descriptor(::move(descriptor))
    , m_value(value)
    , m_meaning(meaning)
    , m_isFinal(isFinal)
  {}
  FieldUpdate(FieldDescriptor descriptor, int64_t value, FieldUpdateMeaning meaning, bool isFinal = true)
    : m_descriptor(::move(descriptor))
    , m_value(value)
    , m_meaning(meaning)
    , m_isFinal(isFinal)
  {}
  FieldUpdate(FieldDescriptor descriptor, float value, FieldUpdateMeaning meaning, bool isFinal = true)
    : m_descriptor(::move(descriptor))
    , m_value(value)
    , m_meaning(meaning)
    , m_isFinal(isFinal)
  {}
  FieldUpdate(FieldDescriptor descriptor, bool value, FieldUpdateMeaning meaning, bool isFinal = true)
    : m_descriptor(::move(descriptor))
    , m_value(value)
    , m_meaning(meaning)
    , m_isFinal(isFinal)
  {}

  FieldUpdate& operator=(const FieldUpdate& rhs) = default;
  // FieldUpdate& operator=(FieldUpdate&& rhs) noexcept = default;

  [[nodiscard]] const FieldDescriptor& descriptor() const { return m_descriptor; }
  [[nodiscard]] const FieldPath& path() const { return m_descriptor.getPath(); }
  [[nodiscard]] FieldUpdateMeaning meaning() const { return m_meaning; }
  [[nodiscard]] const FieldUpdateVariant& value() const { return m_value; }
  [[nodiscard]] bool isIndirect() const { return m_descriptor.isIndirect(); }
  [[nodiscard]] bool needsAutoComplete() const { return m_descriptor.needsAutoComplete(); }
  [[nodiscard]] bool isRequest() const { return m_descriptor.isRequest(); }
  [[nodiscard]] bool isFinal() const { return m_isFinal; }

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
  FieldDescriptor m_descriptor;
  FieldUpdateVariant m_value;
  FieldUpdateMeaning m_meaning;
  bool m_isFinal;
};

#ifdef USE_ETL
using FieldUpdateVector = etl::vector<FieldUpdate, MAX_SETTING_UPDATE_SEQUENCE>;
#else
using FieldUpdateVector = std::vector<FieldUpdate>;
#endif