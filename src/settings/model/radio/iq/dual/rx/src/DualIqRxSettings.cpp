#include "settings/model/radio/iq/DualIqRxSettings.h"
#ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/update/DualIqRxTagLookup.h>
#include <settings/model/update/resolveDottedString.h>
#endif

DualIqRxSettings::DualIqRxSettings()
  : DualIqRxSettingsBaseType()
{
}

#ifdef USE_DOTTED_STRING_PATHS
ResolveDottedStringFunc
DualIqRxSettings::resolveDottedStringFunc()
{
  return [](const char* dottedPath, FieldDescriptor& descriptor) -> ResultCode {
    return ::resolveDottedString(dottedPath, dual_iq_rx_radio_fields, descriptor);
  };
}
#endif