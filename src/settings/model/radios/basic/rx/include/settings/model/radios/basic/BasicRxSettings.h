#pragma once
#include <settings/model/radios/BandSettingsCacheT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/data/radio/RadioLookup.h>
#include <settings/model/radios/RadioSettingsBaseT.h>
#include <settings/model/radios/basic/BasicActiveBandSettings.h>
#include <settings/model/radios/basic/BasicBandSettingsCache.h>


using BasicRxSettingsBaseType = RadioSettingsBaseT<
    makesdr_BasicRxSettingsPb,
    &makesdr_BasicRxSettingsPb_msg,
    makesdr_BasicRxSettingsPayloadPb,
    makesdr_RadioPayloadType_PAYLOAD_SETTINGS_BASIC_RX,
    makesdr_BasicRxSettingsPayloadPb_size,
    BasicActiveBandSettings,
    BasicBandSettingsCache
  >;

class BasicRxSettings : public BasicRxSettingsBaseType
{
public:
  BasicRxSettings();
  BasicRxSettings(const BasicRxSettings& other) = default;
  BasicRxSettings(BasicRxSettings&& other) noexcept = default;
  ~BasicRxSettings() override = default;

#ifdef USE_DOTTED_STRING_PATHS
  ResolveDottedStringFunc resolveDottedStringFunc() override;
#endif
};
