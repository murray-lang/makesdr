#include "settings/model/radios/iq/BasicIqRxTxSettings.h"
#ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/path/BasicIqRxTxTagLookup.h>
#include <settings/model/path/resolveDottedString.h>
#endif

BasicIqRxTxSettings::BasicIqRxTxSettings()
  : BasicIqRxTxSettingsBaseType()
  , m_transmitterSettings(m_payload.body.transmitter)
{
}

#ifdef USE_DOTTED_STRING_PATHS
ResolveDottedStringFunc
BasicIqRxTxSettings::resolveDottedStringFunc()
{
  return [](const char* dottedPath, FieldDescriptor& descriptor) -> ResultCode {
    return ::resolveDottedString(dottedPath, basic_iq_rxtx_radio_fields, descriptor);
  };
}
#endif
