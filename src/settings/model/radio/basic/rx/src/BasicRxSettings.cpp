#include "settings/model/radio/basic/BasicRxSettings.h"

#ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/update/BasicRxTagLookup.h>
#include <settings/model/update/resolveDottedString.h>
#endif

BasicRxSettings::BasicRxSettings()
  : BasicRxSettingsBaseType()
{}

#ifdef USE_DOTTED_STRING_PATHS
ResolveDottedStringFunc
BasicRxSettings::resolveDottedStringFunc()
{
  return [](const char* dottedPath, FieldDescriptor& descriptor) -> ResultCode {
    return ::resolveDottedString(dottedPath, basic_rx_radio_fields, descriptor);
  };
}
#endif