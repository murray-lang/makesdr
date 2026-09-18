#pragma once

#include <settings/model/radios/BandSettingsCacheT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>


using DualIqBandSettingsCache = BandSettingsCacheT<
  makesdr_DualIqBandSettingsCachePb,
  &makesdr_DualIqBandSettingsCachePb_msg,
  makesdr_RadioCacheDualIqPayloadPb,
  makesdr_RadioPayloadType_PAYLOAD_CACHE_DUAL_IQ,
  makesdr_RadioCacheDualIqPayloadPb_size,
  makesdr_DualIqBandSettingsPb,
  makesdr_DualIqBandSettingsCachePb_BandSettingsEntry
>;
