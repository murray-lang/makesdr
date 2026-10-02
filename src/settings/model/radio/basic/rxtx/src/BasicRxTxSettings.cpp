#include "settings/model/radio/basic/BasicRxTxSettings.h"
#ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/update/BasicRxTxTagLookup.h>
#include <settings/model/update/resolveDottedString.h>
#endif

BasicRxTxSettings::BasicRxTxSettings()
  : BasicRxTxSettingsBaseType()
  , m_transmitterSettings(m_payload.body.transmitter)
{}

#ifdef USE_DOTTED_STRING_PATHS
ResolveDottedStringFunc
BasicRxTxSettings::resolveDottedStringFunc()
{
  return [](const char* dottedPath, FieldDescriptor& descriptor) -> ResultCode {
    return ::resolveDottedString(dottedPath, basic_rxtx_radio_fields, descriptor);
  };
}
#endif