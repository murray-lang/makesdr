#include "settings/model/radio/iq/SingleIqRxTxSettings.h"
#ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/update/SingleIqRxTxTagLookup.h>
#include <settings/model/update/resolveDottedString.h>
#endif

SingleIqRxTxSettings::SingleIqRxTxSettings()
  : SingleIqRxTxSettingsBaseType()
  , m_transmitterSettings(m_payload.body.transmitter)
{

}

#ifdef USE_DOTTED_STRING_PATHS
ResolveDottedStringFunc
SingleIqRxTxSettings::resolveDottedStringFunc()
{
  return [](const char* dottedPath, FieldDescriptor& descriptor) -> ResultCode {
    return ::resolveDottedString(dottedPath, basic_iq_rxtx_radio_fields, descriptor);
  };
}
#endif
