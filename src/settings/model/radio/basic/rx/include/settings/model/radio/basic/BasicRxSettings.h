#pragma once
#include <settings/model/radio/BandSettingsCacheT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/radio/RadioSettingsBaseT.h>
#include <settings/model/radio/basic/BasicActiveBandSettings.h>
#include <settings/model/radio/basic/BasicBandSettingsCache.h>


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
  struct Traits
  {
    static constexpr bool hasRx = true;
    static constexpr bool hasTx = false;
    static constexpr bool isDualIq = false;
    static constexpr bool isSplitBand = false;
  };

  BasicRxSettings();
  BasicRxSettings(const BasicRxSettings& other) = default;
  BasicRxSettings(BasicRxSettings&& other) noexcept = default;
  ~BasicRxSettings() override = default;
  BasicRxSettings& operator=(const BasicRxSettings& other);

  [[nodiscard]] RadioSettingsTraits traits() const { return toRuntimeTraits<Traits>(); }

#ifdef USE_DOTTED_STRING_PATHS
  ResolveDottedStringFunc resolveDottedStringFunc();
#endif
};
