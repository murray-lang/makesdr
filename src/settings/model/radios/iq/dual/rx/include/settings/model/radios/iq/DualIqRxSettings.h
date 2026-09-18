#pragma once
#include <settings/model/radios/BandSettingsCacheT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/data/radio/RadioLookup.h>
#include <settings/model/radios/RadioSettingsBaseT.h>
#include <settings/model/radios/iq/DualIqActiveBandSettings.h>
#include <settings/model/radios/iq/DualIqBandSettingsCache.h>

using DualIqRxSettingsBaseType = RadioSettingsBaseT<
    makesdr_DualIqRxSettingsPb,
    &makesdr_DualIqRxSettingsPb_msg,
    makesdr_DualIqRxSettingsPayloadPb,
    makesdr_RadioPayloadType_PAYLOAD_SETTINGS_DUAL_IQ_RX,
    makesdr_DualIqRxSettingsPayloadPb_size,
    DualIqActiveBandSettings,
    DualIqBandSettingsCache
  >;

class DualIqRxSettings : public DualIqRxSettingsBaseType
{
public:
  using Proto = makesdr_DualIqRxSettingsPb;
  using Payload = makesdr_DualIqRxSettingsPayloadPb;
  using Cache = DualIqBandSettingsCache;

  DualIqRxSettings();

#ifdef USE_DOTTED_STRING_PATHS
  ResolveDottedStringFunc resolveDottedStringFunc() override;
#endif

};