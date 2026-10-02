#pragma once

#include <ResultCode.h>
#include <settings/model/update/FieldDescriptor.h>
#include <settings/model/radio/basic/WithBandT.h>
#include <settings/model/radio/basic/WithModeT.h>
#include <settings/model/radio/BandRfSettings.h>
#include <settings/model/radio/IfSettings.h>
#include <settings/model/radio/AgcSpeed.h>
#include <settings/model/radio/Mode.h>
#include <settings/model/radio/IBandSettings.h>

#include <settings/model/meta/radio/RadioLookup.h>

#include "BasicBandSettingsCache.h"


class BasicBandSettings :
  public IBandSettings
  , public WithBandT<
    makesdr_BasicBandSettingsPb,
    makesdr_BasicBandSettingsPb_band_request_tag,
    makesdr_BasicBandSettingsPb_band_tag,
    BasicBandSettingsCache
  >
  , public WithModeT<
    makesdr_BasicBandSettingsPb,
    makesdr_BasicBandSettingsPb_mode_request_tag,
    makesdr_BasicBandSettingsPb_mode_tag
  >
{
public:
  BasicBandSettings(makesdr_BasicBandSettingsPb& rawSettings);

  [[nodiscard]] const Band * getBand() const override { return &m_band; }
  [[nodiscard]] const Mode* getFocusMode() const override { return &m_mode; }

  [[nodiscard]] bool hasRfSettings() const override { return m_rawSettings.has_rf; }
  BandRfSettings* rfSettings() override { return &m_rfSettings; }
  [[nodiscard]] const BandRfSettings* rfSettings() const override { return &m_rfSettings; }

  [[nodiscard]] bool hasIfSettings() const override { return m_rawSettings.has_if_; }
  IfSettings* ifSettings() override { return &m_ifSettings; }
  [[nodiscard]] const IfSettings* ifSettings() const override { return &m_ifSettings; }

  [[nodiscard]] bool hasAgcSpeed() const { return m_rawSettings.has_agc_speed; }
  [[nodiscard]] AgcSpeed agcSpeed() const { return static_cast<AgcSpeed>(m_rawSettings.agc_speed); }

  ResultCode updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex) override
  {
    return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID; // No indirect fields in "Basic" classes
  }

  ResultCode autoComplete(
    const BandCategoryList* bands, const ModeList* modes, BasicBandSettingsCache* cache);

  ResultCode autoComplete(
    const FieldDescriptor& setting,
    uint32_t startIndex,
    const BandCategoryList* bands,
    const ModeList* modes,
    BasicBandSettingsCache* cache
    );

  ResultCode applyBandDefaults(const Band& band, const BandCategoryList* bands, const ModeList* modes) override;

protected:
  makesdr_BasicBandSettingsPb& m_rawSettings;

  BandRfSettings m_rfSettings;
  IfSettings m_ifSettings;
};
