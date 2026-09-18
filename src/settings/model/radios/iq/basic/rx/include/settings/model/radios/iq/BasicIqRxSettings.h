#pragma once
#include <settings/model/radios/BandSettingsCacheT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/data/radio/RadioLookup.h>
#include <settings/model/radios/RadioSettingsBaseT.h>
#include <settings/model/radios/iq/BasicIqActiveBandSettings.h>
#include <settings/model/radios/iq/BasicIqBandSettingsCache.h>

using BasicIqRxSettingsBaseType = RadioSettingsBaseT<
    makesdr_BasicIqRxSettingsPb,
    &makesdr_BasicIqRxSettingsPb_msg,
    makesdr_BasicIqRxSettingsPayloadPb,
    makesdr_RadioPayloadType_PAYLOAD_SETTINGS_BASIC_IQ_RX,
    makesdr_BasicIqRxSettingsPayloadPb_size,
    BasicIqActiveBandSettings,
    BasicIqBandSettingsCache
  >;

class BasicIqRxSettings : public BasicIqRxSettingsBaseType
{
public:
  BasicIqRxSettings();

#ifdef USE_DOTTED_STRING_PATHS
  ResolveDottedStringFunc resolveDottedStringFunc() override;
#endif

};