#include "settings/model/radios/iq/DualIqRxTxSettings.h"
#ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/path/DualIqRxTxTagLookup.h>
#include <settings/model/path/resolveDottedString.h>
#endif

DualIqRxTxSettings::DualIqRxTxSettings()
  : DualIqRxTxSettingsBaseType()
  , m_transmitterSettings(m_payload.body.transmitter)
{
}

#ifdef USE_DOTTED_STRING_PATHS
ResolveDottedStringFunc
DualIqRxTxSettings::resolveDottedStringFunc()
{
  return [](const char* dottedPath, FieldDescriptor& descriptor) -> ResultCode {
    return ::resolveDottedString(dottedPath, dual_iq_rxtx_radio_fields, descriptor);
  };
}
#endif

