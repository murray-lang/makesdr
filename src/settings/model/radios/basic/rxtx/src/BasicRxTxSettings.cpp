#include "settings/model/radios/basic/BasicRxTxSettings.h"
#ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/path/BasicRxTxTagLookup.h>
#include <settings/model/path/resolveDottedString.h>
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