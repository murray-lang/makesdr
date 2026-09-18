#include "settings/model/message/FieldUpdate.h"
#include "settings/model/message/FieldUpdateMessage.h"

FieldUpdate::FieldUpdate(const FieldUpdateMessage& message)
  : m_descriptor(
    message.path(),
    message.body().path_count,
    message.needsAutoComplete(),
    message.isIndirect()
    )
  , m_meaning(message.meaning())
  , m_isFinal(message.isFinal())
{
  message.getValue(m_value);
}
