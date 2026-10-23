#pragma once
#include <settings/model/radio/BandSettingsCacheT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/radio/RadioSettingsBaseT.h>
#include <settings/model/radio/iq/SingleIqActiveBandSettings.h>
#include <settings/model/radio/iq/SingleIqBandSettingsCache.h>

#ifdef USE_DOTTED_STRING_PATHS
#include "settings/model/update/ResolveDottedStringFunc.h"
#endif

using SingleIqRxSettingsBaseType = RadioSettingsBaseT<
    makesdr_SingleIqRxSettingsPb,
    &makesdr_SingleIqRxSettingsPb_msg,
    makesdr_SingleIqRxSettingsPayloadPb,
    makesdr_RadioPayloadType_PAYLOAD_SETTINGS_SINGLE_IQ_RX,
    makesdr_SingleIqRxSettingsPayloadPb_size,
    SingleIqActiveBandSettings,
    SingleIqBandSettingsCache
  >;

class SingleIqRxSettings : public SingleIqRxSettingsBaseType
{
public:

  using ActiveBandSettings = SingleIqActiveBandSettings;

  struct Traits
  {
    static constexpr bool hasRx = true;
    static constexpr bool hasTx = false;
    static constexpr bool isDualIq = false;
    static constexpr bool isSplitBand = false;
  };

  SingleIqRxSettings();
  SingleIqRxSettings(const SingleIqRxSettings& other) = default;
  SingleIqRxSettings& operator=(const SingleIqRxSettings& other);

  [[nodiscard]] RadioSettingsTraits traits() const { return toRuntimeTraits<Traits>(); }

#ifdef USE_DOTTED_STRING_PATHS
  ResolveDottedStringFunc resolveDottedStringFunc();
#endif

};