#include "settings/model/message/FieldUpdateMessage.h"
#include <cstring>


FieldUpdateMessage::FieldUpdateMessage(const FieldUpdate& field, PayloadSource source)
{
  m_deemComplete = true;
  m_payload.header.payloadType = makesdr_RadioPayloadType_PAYLOAD_FIELD_UPDATE;
  m_payload.header.source = static_cast<makesdr_RadioPayloadSource>(source);
  m_payload.header.purpose = makesdr_RadioPayloadPurpose_PURPOSE_REPLACE;
  setValue(field.value());
  m_payload.body.path_count = field.path().size();
  for (size_t i = 0; i < field.path().size(); i++) {
    m_payload.body.path[i] = field.path()[i];
  }
  m_payload.body.is_auto_complete = field.needsAutoComplete();
  m_payload.body.is_indirect = field.isIndirect();
  m_payload.body.meaning = static_cast<makesdr_FieldUpdateMeaning>(field.meaning());
  m_payload.body.is_final = field.isFinal();
}


bool
FieldUpdateMessage::getValue(FieldUpdateVariant& var) const
{
  switch (m_payload.body.which_value_variant) {
  case makesdr_FieldUpdatePb_string_value_tag:
    var.emplace<NameString>(m_payload.body.value_variant.string_value);
    break;
  case makesdr_FieldUpdatePb_int32_value_tag:
    var.emplace<int32_t>(m_payload.body.value_variant.int32_value);
    break;
  case makesdr_FieldUpdatePb_uint32_value_tag:
    var.emplace<uint32_t>(m_payload.body.value_variant.uint32_value);
    break;
  case makesdr_FieldUpdatePb_int64_value_tag:
    var.emplace<int64_t>(m_payload.body.value_variant.int64_value);
    break;
  case makesdr_FieldUpdatePb_float_value_tag:
    var.emplace<float>(m_payload.body.value_variant.float_value);
    break;
  case makesdr_FieldUpdatePb_bool_value_tag:
    var.emplace<bool>(m_payload.body.value_variant.bool_value);
    break;
  default:
    return false;
  }
  return true;
}

bool
FieldUpdateMessage::setValue(const FieldUpdateVariant& var)
{
  return ::visit([this](auto&& value) ->bool {
    using T = std::decay_t<decltype(value)>;
    if constexpr (std::is_same_v<T, NameString>) {
      m_payload.body.which_value_variant = makesdr_FieldUpdatePb_string_value_tag;
      strncpy(m_payload.body.value_variant.string_value, value.c_str(), sizeof(m_payload.body.value_variant.string_value) - 1);
      m_payload.body.value_variant.string_value[sizeof(m_payload.body.value_variant.string_value) - 1] = '\0';
    } else if constexpr (std::is_same_v<T, int32_t>) {
      m_payload.body.which_value_variant = makesdr_FieldUpdatePb_int32_value_tag;
      m_payload.body.value_variant.int32_value = value;
    } else if constexpr (std::is_same_v<T, uint32_t>) {
      m_payload.body.which_value_variant = makesdr_FieldUpdatePb_uint32_value_tag;
      m_payload.body.value_variant.uint32_value = value;
    } else if constexpr (std::is_same_v<T, int64_t>) {
      m_payload.body.which_value_variant = makesdr_FieldUpdatePb_int64_value_tag;
      m_payload.body.value_variant.int64_value = value;
    } else if constexpr (std::is_same_v<T, float>) {
      m_payload.body.which_value_variant = makesdr_FieldUpdatePb_float_value_tag;
      m_payload.body.value_variant.float_value = value;
    } else if constexpr (std::is_same_v<T, bool>) {
      m_payload.body.which_value_variant = makesdr_FieldUpdatePb_bool_value_tag;
      m_payload.body.value_variant.bool_value = value;
    } else {
      return false;
    }
    return true;
  }, var);

}