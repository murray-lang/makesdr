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
  RadioSettings(RadioSettings_RadioSettingsPb& raw)
    : m_rawSettings(raw)
    , m_visitor(&raw, &RadioSettings_RadioSettingsPb_msg)
  {
    InitBandAndPipelineIdsWithDefaults();
  }

  ResultCode updateField(const SettingFieldUpdate &settingUpdate)
  {
    return m_visitor.updateField(settingUpdate);
  }

  ResultCode updateField(const SettingFieldPath &path, const SettingFieldVariant &value)
  {
    return m_visitor.updateField(path, value);
  }

  ResultCode getField(const SettingFieldPath &path, SettingFieldVariant &value) const
  {
    return m_visitor.getField(path, value);
  }

  ResultCode getField(
    const SettingFieldPath &path,
    SettingFieldVariant &value,
    bool mustHave,
    bool parentsMustHave,
    bool& retrieved
  )
  {
    return m_visitor.getField(path, value, mustHave, parentsMustHave, retrieved);
  }

  ResultCode setFieldPresence(const SettingFieldPath &path, bool present)
  {
    return m_visitor.setFieldPresence(path, present);
  }

  ResultCode mergePresentFields(const void* pRhsMessage)
  {
    return m_visitor.mergePresentFields(pRhsMessage);
  }

  static ResultCode resolveDottedPath(
    const char *dottedPath,
    SettingFieldPath &path,
    bool* isIndirectOut,
    AutoCompleteTrigger* triggerOut
  )
  {
    return MessageVisitor::resolveDottedPath(dottedPath, path, isIndirectOut, triggerOut);
  }

  [[nodiscard]] const RadioSettings_BandSettingsPb* getBandSettings(SplitBandId bandId) const
  {
    switch (bandId)
    {
    case SplitBandId::One: return &m_rawSettings.active_bands.band_1;
    case SplitBandId::Two: return &m_rawSettings.active_bands.band_2;
    default: return nullptr;
    }
  }

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

  void ptt(bool on)
  {
    m_rawSettings.ptt = on;
    m_rawSettings.has_ptt = true;
  }

  bool hasActiveBands() const { return m_rawSettings.has_active_bands; }

  [[nodiscard]] SplitBandId getFocusBandId() const { return static_cast<SplitBandId>(m_rawSettings.active_bands.focus_band_id); }
  [[nodiscard]] SplitBandId getRxBandId() const { return static_cast<SplitBandId>(m_rawSettings.active_bands.rx_band_id); }
  [[nodiscard]] SplitBandId getTxBandId() const { return static_cast<SplitBandId>(m_rawSettings.active_bands.tx_band_id); }

  [[nodiscard]] PipelineId getFocusPipelineId(SplitBandId bandId) const
  {
    switch (bandId) {
    case SplitBandId::One: return static_cast<PipelineId>(m_rawSettings.active_bands.band_1.focus_pipeline_id);
    case SplitBandId::Two: return static_cast<PipelineId>(m_rawSettings.active_bands.band_2.focus_pipeline_id);
    default: return PipelineId::NONE;
    }
  }

  [[nodiscard]] PipelineId getFocusBandFocusPipelineId() const
  {
    switch (m_rawSettings.active_bands.focus_band_id) {
    case RadioSettings_SplitBandId_SPLIT_BAND_ONE:
      return static_cast<PipelineId>(m_rawSettings.active_bands.band_1.focus_pipeline_id);
    case RadioSettings_SplitBandId_SPLIT_BAND_TWO:
      return static_cast<PipelineId>(m_rawSettings.active_bands.band_2.focus_pipeline_id);
    default: return PipelineId::NONE;
    }
  }

  [[nodiscard]] const RadioSettings_BandSettingsPb* getFocusBandSettings() const
  {
    return getBandSettings(static_cast<SplitBandId>(m_rawSettings.active_bands.focus_band_id));
  }

  [[nodiscard]] const RadioSettings_BandSettingsPb* getTxBandSettings() const
  {
    return getBandSettings(static_cast<SplitBandId>(m_rawSettings.active_bands.tx_band_id));
  }

  [[nodiscard]] const RadioSettings_RxPipelineSettingsPb* getFocusRxPipelineSettings(SplitBandId bandId) const
  {
    const RadioSettings_BandSettingsPb* pBandSettings = getBandSettings(bandId);
    if (pBandSettings == nullptr) return nullptr;
    switch (pBandSettings->focus_pipeline_id)
    {
      case RadioSettings_PipelineId_PIPELINE_A: return &pBandSettings->pipeline_a;
      case RadioSettings_PipelineId_PIPELINE_B: return &pBandSettings->pipeline_b;
      default: return nullptr;
    }
  }

  [[nodiscard]] const RadioSettings_RxPipelineSettingsPb* getTxPipelineSettings() const
  {
    const RadioSettings_BandSettingsPb* pBandSettings = getTxBandSettings();
    if (pBandSettings == nullptr) return nullptr;
    switch (pBandSettings->tx_pipeline_id)
    {
    case RadioSettings_PipelineId_PIPELINE_A: return &pBandSettings->pipeline_a;
    case RadioSettings_PipelineId_PIPELINE_B: return &pBandSettings->pipeline_b;
    default: return nullptr;
    }
  }

  [[nodiscard]] const RadioSettings_RxPipelineSettingsPb* getFocusBandFocusRxPipelineSettings() const
  {
    return getFocusRxPipelineSettings(static_cast<SplitBandId>(m_rawSettings.active_bands.focus_band_id));
  }


protected:
  void InitBandAndPipelineIdsWithDefaults()
  {
    if (m_rawSettings.active_bands.focus_band_id == RadioSettings_SplitBandId_SPLIT_BAND_NONE) {
      m_rawSettings.active_bands.focus_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
      m_rawSettings.active_bands.has_focus_band_id = true;
    }
    if (m_rawSettings.active_bands.rx_band_id == RadioSettings_SplitBandId_SPLIT_BAND_NONE) {
      m_rawSettings.active_bands.rx_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
      m_rawSettings.active_bands.has_rx_band_id = true;
    }
    if (m_rawSettings.active_bands.tx_band_id == RadioSettings_SplitBandId_SPLIT_BAND_NONE) {
      m_rawSettings.active_bands.tx_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
      m_rawSettings.active_bands.has_tx_band_id = true;
    }
    if (m_rawSettings.active_bands.band_1.focus_pipeline_id == RadioSettings_PipelineId_PIPELINE_NONE) {
      m_rawSettings.active_bands.band_1.focus_pipeline_id = RadioSettings_PipelineId_PIPELINE_A;
      m_rawSettings.active_bands.band_1.has_focus_pipeline_id = true;
    }
    if (m_rawSettings.active_bands.band_2.focus_pipeline_id == RadioSettings_PipelineId_PIPELINE_NONE) {
      m_rawSettings.active_bands.band_2.focus_pipeline_id = RadioSettings_PipelineId_PIPELINE_A;
      m_rawSettings.active_bands.band_2.has_focus_pipeline_id = true;
    }
    if (m_rawSettings.active_bands.band_1.tx_pipeline_id == RadioSettings_PipelineId_PIPELINE_NONE) {
      m_rawSettings.active_bands.band_1.tx_pipeline_id = RadioSettings_PipelineId_PIPELINE_A;
      m_rawSettings.active_bands.band_1.has_tx_pipeline_id = true;
    }
    if (m_rawSettings.active_bands.band_2.tx_pipeline_id == RadioSettings_PipelineId_PIPELINE_NONE) {
      m_rawSettings.active_bands.band_2.tx_pipeline_id = RadioSettings_PipelineId_PIPELINE_A;
      m_rawSettings.active_bands.band_2.has_tx_pipeline_id = true;
    }
  }

protected:
  RadioSettings_RadioSettingsPb& m_rawSettings;

  MessageVisitor m_visitor;
};