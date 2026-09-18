#pragma once
#include <settings/model/radios/BandSettingsCacheT.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/proto/RadioSettings.pb.h>

using BasicIqBandSettingsCache = BandSettingsCacheT<
  makesdr_BasicIqBandSettingsCachePb,
  &makesdr_BasicIqBandSettingsCachePb_msg,
  makesdr_RadioCacheBasicIqPayloadPb,
  makesdr_RadioPayloadType_PAYLOAD_CACHE_BASIC_IQ,
  makesdr_RadioCacheBasicIqPayloadPb_size,
  makesdr_BasicIqBandSettingsPb,
  makesdr_BasicIqBandSettingsCachePb_BandSettingsEntry
>;
