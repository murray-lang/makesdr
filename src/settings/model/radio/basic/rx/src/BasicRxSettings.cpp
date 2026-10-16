#include "settings/model/radio/basic/BasicRxSettings.h"

#ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/update/BasicRxTagLookup.h>
#include <settings/model/update/resolveDottedString.h>
#endif

BasicRxSettings::BasicRxSettings()
  : BasicRxSettingsBaseType()
{
  m_payload.body.capabilities.rx = true;
  m_payload.body.capabilities.has_tx = false;
  m_payload.body.capabilities.has_iq = false;
}

BasicRxSettings&
BasicRxSettings::operator=(const BasicRxSettings& other)
{
  if (this != &other) {
    replace(other.body(), true);
  }
  return *this;
}

#ifdef USE_DOTTED_STRING_PATHS
ResolveDottedStringFunc
BasicRxSettings::resolveDottedStringFunc()
{
  return [](const char* dottedPath, FieldDescriptor& descriptor) -> ResultCode {
    return ::resolveDottedString(dottedPath, basic_rx_radio_fields, descriptor);
  };
}
#endif