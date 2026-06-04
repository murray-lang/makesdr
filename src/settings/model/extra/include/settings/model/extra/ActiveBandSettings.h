#pragma once

#include "settings/model/core/SettingsBase.h"
#include "settings/model/core/SettingFieldPath.h"
#include "settings/model/core/AutoCompleteTrigger.h"
#include "settings/model/proto/RadioSettings.pb.h"
#include "BandSettings.h"
#include "../../../../../core/include/settings/model/core/SplitBandId.h"

class ActiveBandSettings : public SettingsBase, public ResolveIndirection, public AutoComplete
{
public:
  ActiveBandSettings(RadioSettings_ActiveBandSettingsPb& raw)
    : m_rawSettings(raw)
    , m_bandOneSettings(raw.band_1)
    , m_bandTwoSettings(raw.band_2)
  {}

  [[nodiscard]] bool hasIsSplit() const { return m_rawSettings.has_is_split; }
  [[nodiscard]] bool hasFocusBandId() const { return m_rawSettings.has_focus_band_id; }
  [[nodiscard]] bool hasTxBandId() const { return m_rawSettings.has_tx_band_id; }
  [[nodiscard]] bool hasRxBandId() const { return m_rawSettings.has_rx_band_id; }
  [[nodiscard]] bool hasBand1() const { return m_rawSettings.has_band_1; }
  [[nodiscard]] bool hasBand2() const { return m_rawSettings.has_band_2; }

  [[nodiscard]] bool isSplit() const { return m_rawSettings.is_split; }
  [[nodiscard]] SplitBandId focusBandId() const { return static_cast<SplitBandId>(m_rawSettings.focus_band_id); }
  [[nodiscard]] SplitBandId txBandId() const { return static_cast<SplitBandId>(m_rawSettings.tx_band_id); }
  [[nodiscard]] SplitBandId rxBandId() const { return static_cast<SplitBandId>(m_rawSettings.rx_band_id); }

  BandSettings* getBandSettings(SplitBandId bandId)
  {
    if (bandId == SplitBandId::One) return &m_bandOneSettings;
    if (bandId == SplitBandId::Two) return &m_bandTwoSettings;
    return nullptr;
  }

  [[nodiscard]] const BandSettings* getBandSettings(SplitBandId bandId) const
  {
    if (bandId == SplitBandId::One) return &m_bandOneSettings;
    if (bandId == SplitBandId::Two) return &m_bandTwoSettings;
    return nullptr;
  }

  BandSettings* getFocusBandSettings() { return getBandSettings(static_cast<SplitBandId>(m_rawSettings.focus_band_id)); }
  BandSettings* getTxBandSettings() { return getBandSettings(static_cast<SplitBandId>(m_rawSettings.tx_band_id)); }
  BandSettings* getRxBandSettings() { return getBandSettings(static_cast<SplitBandId>(m_rawSettings.rx_band_id)); }

  [[nodiscard]] const BandSettings* getFocusBandSettings() const
  {
    return const_cast<ActiveBandSettings*>(this)->getFocusBandSettings();
  }
  [[nodiscard]] const BandSettings* getTxBandSettings() const
  {
    return const_cast<ActiveBandSettings*>(this)->getTxBandSettings();
  }
  [[nodiscard]] const BandSettings* getRxBandSettings() const
  {
    return const_cast<ActiveBandSettings*>(this)->getRxBandSettings();
  }

  BandSettings& bandOneSettings() { return m_bandOneSettings; }
  BandSettings& bandTwoSettings() { return m_bandTwoSettings; }
  [[nodiscard]] const BandSettings& bandOneSettings() const { return m_bandOneSettings; }
  [[nodiscard]] const BandSettings& bandTwoSettings() const { return m_bandTwoSettings; }

  ResultCode resolveIndirection(
    const SettingFieldPath& indirectPath,
    uint32_t startingAtIndex,
    SettingFieldPath& resolvedPath
  ) override
  {
    if (startingAtIndex >= indirectPath.size()) {
      return ResultCode::ERR_SETTING_RESOLVE_INDIRECTION_PATH_INVALID;
    }
    switch (indirectPath[startingAtIndex]) {
    case RadioSettings_ActiveBandSettingsPb_focus_band_tag:
      {
        SplitBandId bandId = focusBandId();
        if (bandId == SplitBandId::One) {
          resolvedPath.emplace_back(RadioSettings_ActiveBandSettingsPb_band_1_tag);
          return m_bandOneSettings.resolveIndirection(indirectPath, startingAtIndex + 1, resolvedPath);
        }
        if (bandId == SplitBandId::Two) {
          resolvedPath.emplace_back(RadioSettings_ActiveBandSettingsPb_band_2_tag);
          return m_bandTwoSettings.resolveIndirection(indirectPath, startingAtIndex + 1, resolvedPath);
        }
        return ResultCode::ERR_SETTING_BAND_SETTINGS_FOCUS_BAND_NOT_SET;
      }
      break;
    default: break;
    }
    for (uint32_t i = startingAtIndex; i < indirectPath.size(); i++) {
      resolvedPath.emplace_back(indirectPath[i]);
    }
    return ResultCode::OK;
  }

  void setCategories(RadioCategories* categories)
  {
    m_bandOneSettings.setCategories(categories);
    m_bandTwoSettings.setCategories(categories);
  }

  ResultCode autoComplete(
    const SettingFieldPath& path,
    uint32_t startingAtIndex,
    AutoCompleteTrigger trigger) override;

  ResultCode autoComplete() override;

protected:
  ResultCode autoCompleteSplit();

  RadioSettings_ActiveBandSettingsPb& m_rawSettings;
  BandSettings m_bandOneSettings;
  BandSettings m_bandTwoSettings;

};
