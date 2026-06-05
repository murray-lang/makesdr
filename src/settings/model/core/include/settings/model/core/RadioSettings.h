#pragma once


#include "SettingsBase.h"
#include "SettingFieldPath.h"
#include "SettingFieldVariant.h"
#include "MessageVisitor.h"
#include "SplitBandId.h"
#include "PipelineId.h"

class RadioSettings : public SettingsBase
{
public:
  RadioSettings(RadioSettings_RadioSettingsPb& raw);

  ResultCode updateField(const SettingFieldUpdate &settingUpdate);
  ResultCode updateField(const SettingFieldPath &path, const SettingFieldVariant &value);
  ResultCode getField(const SettingFieldPath &path, SettingFieldVariant &value) const;
  ResultCode getField(
    const SettingFieldPath &path,
    SettingFieldVariant &value,
    bool mustHave,
    bool parentsMustHave,
    bool& retrieved
  );

  ResultCode setFieldPresence(const SettingFieldPath &path, bool present);
  ResultCode mergePresentFields(const void* pRhsMessage);

  static ResultCode resolveDottedPath(
    const char *dottedPath,
    SettingFieldPath &path,
    bool* isIndirectOut,
    AutoCompleteTrigger* triggerOut
  );

  [[nodiscard]] const RadioSettings_BandSettingsPb* getBandSettings(SplitBandId bandId) const;

  // [[nodiscard]] uint32_t getFocusBandTag() const
  // {
  //   switch (m_rawSettings.active_bands.focus_band_id)
  //   {
  //   case RadioSettings_SplitBandId_SPLIT_BAND_ONE: return RadioSettings_ActiveBandSettingsPb_band_1_tag;
  //   case RadioSettings_SplitBandId_SPLIT_BAND_TWO: return RadioSettings_ActiveBandSettingsPb_band_2_tag;
  //   default: return 0;
  //   }
  // }
  //
  // [[nodiscard]] uint32_t getFocusPipelineTag(uint32_t bandTag) const
  // {
  //   switch(bandTag) {
  //     case RadioSettings_ActiveBandSettingsPb_band_1_tag:
  //     return getPipelineTag(static_cast<PipelineId>(m_rawSettings.active_bands.band_1.focus_pipeline_id));
  //     case RadioSettings_ActiveBandSettingsPb_band_2_tag:
  //     return getPipelineTag(static_cast<PipelineId>(m_rawSettings.active_bands.band_2.focus_pipeline_id));
  //     default: return 0;
  //   }
  // }
  //
  // [[nodiscard]] uint32_t getPipelineTag(PipelineId pipelineId) const
  // {
  //   switch (pipelineId) {
  //   case PipelineId::A: return RadioSettings_BandSettingsPb_pipeline_a_tag;
  //   case PipelineId::B: return RadioSettings_BandSettingsPb_pipeline_b_tag;
  //   default: return 0;
  //   }
  // }

  void ptt(bool on);
  bool hasActiveBands() const { return m_rawSettings.has_active_bands; }

  [[nodiscard]] SplitBandId getFocusBandId() const { return static_cast<SplitBandId>(m_rawSettings.active_bands.focus_band_id); }
  [[nodiscard]] SplitBandId getRxBandId() const { return static_cast<SplitBandId>(m_rawSettings.active_bands.rx_band_id); }
  [[nodiscard]] SplitBandId getTxBandId() const { return static_cast<SplitBandId>(m_rawSettings.active_bands.tx_band_id); }

  [[nodiscard]] PipelineId getFocusPipelineId(SplitBandId bandId) const;
  [[nodiscard]] PipelineId getFocusBandFocusPipelineId() const;
  [[nodiscard]] const RadioSettings_BandSettingsPb* getFocusBandSettings() const;
  [[nodiscard]] const RadioSettings_BandSettingsPb* getTxBandSettings() const;
  [[nodiscard]] const RadioSettings_RxPipelineSettingsPb* getFocusRxPipelineSettings(SplitBandId bandId) const;
  [[nodiscard]] const RadioSettings_RxPipelineSettingsPb* getTxPipelineSettings() const;
  [[nodiscard]] const RadioSettings_RxPipelineSettingsPb* getFocusBandFocusRxPipelineSettings() const;

protected:
  void InitBandAndPipelineIdsWithDefaults();
  ResultCode resolveIndirection(
    const SettingFieldPath& indirectPath,
    uint32_t startingAtIndex,
    SettingFieldPath& resolvedPath
    );

  ResultCode resolveActiveBandsIndirection(
    const RadioSettings_ActiveBandSettingsPb& rawActiveBands,
    const SettingFieldPath& indirectPath,
    uint32_t startingAtIndex,
    SettingFieldPath& resolvedPath
    );

  ResultCode resolveBandIndirection(
    const RadioSettings_BandSettingsPb& rawBandSettings,
    const SettingFieldPath& indirectPath,
    uint32_t startingAtIndex,
    SettingFieldPath& resolvedPath
  );

protected:
  RadioSettings_RadioSettingsPb& m_rawSettings;

  MessageVisitor m_visitor;
};