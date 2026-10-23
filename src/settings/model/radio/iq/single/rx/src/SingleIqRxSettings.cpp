#include "settings/model/radio/iq/SingleIqRxSettings.h"
#ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/update/SingleIqRxTagLookup.h>
#include <settings/model/update/resolveDottedString.h>
#endif

SingleIqRxSettings::SingleIqRxSettings()
  : SingleIqRxSettingsBaseType()
{
  // m_payload.body.capabilities.rx = true;
  // m_payload.body.capabilities.has_tx = false;
  // m_payload.body.capabilities.has_iq = true;
  // m_payload.body.capabilities.iq.dual_pipeline = false;
}

SingleIqRxSettings&
SingleIqRxSettings::operator=(const SingleIqRxSettings& other)
{
  if (this != &other) {
    replace(other.body(), true);
  }
  return *this;
}

#ifdef USE_DOTTED_STRING_PATHS
ResolveDottedStringFunc
SingleIqRxSettings::resolveDottedStringFunc()
{
  return [](const char* dottedPath, FieldDescriptor& descriptor) -> ResultCode {
    return ::resolveDottedString(dottedPath, basic_iq_rx_radio_fields, descriptor);
  };
}
#endif