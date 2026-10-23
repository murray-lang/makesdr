#include "settings/model/radio/basic/BasicRxTxSettings.h"
#ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/update/BasicRxTxTagLookup.h>
#include <settings/model/update/resolveDottedString.h>
#endif

BasicRxTxSettings::BasicRxTxSettings()
  : BasicRxTxSettingsBaseType()
  , m_transmitterSettings(m_payload.body.transmitter)
{
  m_payload.body.capabilities.rx = true;
  m_payload.body.capabilities.has_tx = true;
  m_payload.body.capabilities.tx.split_band = false;
  m_payload.body.capabilities.has_iq = false;
}

#ifdef USE_DOTTED_STRING_PATHS
ResolveDottedStringFunc
BasicRxTxSettings::resolveDottedStringFunc()
{
  return [](const char* dottedPath, FieldDescriptor& descriptor) -> ResultCode {
    return ::resolveDottedString(dottedPath, basic_rxtx_radio_fields, descriptor);
  };
}
#endif