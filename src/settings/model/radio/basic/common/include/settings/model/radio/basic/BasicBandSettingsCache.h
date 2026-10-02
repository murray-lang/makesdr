#pragma once
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/radio/BandSettingsCacheT.h>

using BasicBandSettingsCache = BandSettingsCacheT<
  makesdr_BasicBandSettingsCachePb,
  &makesdr_BasicBandSettingsCachePb_msg,
  makesdr_RadioCacheBasicPayloadPb,
  makesdr_RadioPayloadType_PAYLOAD_CACHE_BASIC,
  makesdr_RadioCacheBasicPayloadPb_size,
  makesdr_BasicBandSettingsPb,
  makesdr_BasicBandSettingsCachePb_BandSettingsEntry
>;
