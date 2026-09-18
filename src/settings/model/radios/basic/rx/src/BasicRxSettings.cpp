#include "settings/model/radios/basic/BasicRxSettings.h"

#ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/path/BasicRxTagLookup.h>
#include <settings/model/path/resolveDottedString.h>
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