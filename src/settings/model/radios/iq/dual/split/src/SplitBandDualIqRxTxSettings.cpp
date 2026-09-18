#include "settings/model/radios/iq/SplitBandDualIqRxTxSettings.h"
#ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/path/SplitBandDualIqTagLookup.h>
#include <settings/model/path/resolveDottedString.h>
#endif

SplitBandDualIqRxTxSettings::SplitBandDualIqRxTxSettings()
  : SplitBandDualIqRxTxSettingsBaseType()
  // , m_transmitterSettings(m_payload.body.transmitter)
  , m_updater(static_cast<FieldUpdateSink*>(this))
{
  InitBandAndPipelineIdsWithDefaults();
}

SplitBandDualIqRxTxSettings::SplitBandDualIqRxTxSettings(const Payload& payload, bool complete)
  : SplitBandDualIqRxTxSettingsBaseType(payload, complete)
  // , m_transmitterSettings(m_payload.body.transmitter)
  , m_updater(static_cast<FieldUpdateSink*>(this))
{
}

SplitBandDualIqRxTxSettings::SplitBandDualIqRxTxSettings(const SplitBandDualIqRxTxSettings& other)
  : SplitBandDualIqRxTxSettingsBaseType(other)
  // , m_transmitterSettings(m_payload.body.transmitter)
  , m_updater(static_cast<FieldUpdateSink*>(this))
{

}

SplitBandDualIqRxTxSettings::SplitBandDualIqRxTxSettings(SplitBandDualIqRxTxSettings&& other) noexcept
  : SplitBandDualIqRxTxSettingsBaseType(::move(other))
  // , m_transmitterSettings(m_payload.body.transmitter)
  , m_updater(static_cast<FieldUpdateSink*>(this))
{

}

SplitBandDualIqRxTxSettings&
SplitBandDualIqRxTxSettings::operator=(const SplitBandDualIqRxTxSettings& other)
{
  if (this != &other) {
    replace(other.body(), true);
    // SplitBandDualIqRxTxSettingsBaseType::operator=(other);
    // m_transmitterSettings = TransmitterSettings(m_payload.body.transmitter);
    // m_shortcutExpander = SplitBandDualIqRxTxShortcutExpander(static_cast<FieldUpdateSink*>(this));
  }
  return *this;
}
//
// SplitBandDualIqRxTxSettings& SplitBandDualIqRxTxSettings::operator=(SplitBandDualIqRxTxSettings&& other) noexcept
// {
//   if (this != &other) {
//     // SplitBandDualIqRxTxSettingsBaseType::operator=(::move(other));
//     // m_transmitterSettings = TransmitterSettings(m_payload.body.transmitter);
//     // m_shortcutExpander = SplitBandDualIqRxTxShortcutExpander(static_cast<FieldUpdateSink*>(this));
//   }
//   return *this;
// }

void
SplitBandDualIqRxTxSettings::InitBandAndPipelineIdsWithDefaults()
{
  if (m_payload.body.active_bands.focus_band_id == makesdr_SplitBandId_SPLIT_BAND_NONE) {
    m_payload.body.active_bands.focus_band_id = makesdr_SplitBandId_SPLIT_BAND_ONE;
    m_payload.body.active_bands.has_focus_band_id = true;
  }
  if (m_payload.body.active_bands.rx_band_id == makesdr_SplitBandId_SPLIT_BAND_NONE) {
    m_payload.body.active_bands.rx_band_id = makesdr_SplitBandId_SPLIT_BAND_ONE;
    m_payload.body.active_bands.has_rx_band_id = true;
  }
  if (m_payload.body.active_bands.tx_band_id == makesdr_SplitBandId_SPLIT_BAND_NONE) {
    m_payload.body.active_bands.tx_band_id = makesdr_SplitBandId_SPLIT_BAND_ONE;
    m_payload.body.active_bands.has_tx_band_id = true;
  }
  if (m_payload.body.active_bands.band_1.focus_pipeline_id == makesdr_PipelineId_PIPELINE_NONE) {
    m_payload.body.active_bands.band_1.focus_pipeline_id = makesdr_PipelineId_PIPELINE_A;
    m_payload.body.active_bands.band_1.has_focus_pipeline_id = true;
  }
  if (m_payload.body.active_bands.band_2.focus_pipeline_id == makesdr_PipelineId_PIPELINE_NONE) {
    m_payload.body.active_bands.band_2.focus_pipeline_id = makesdr_PipelineId_PIPELINE_A;
    m_payload.body.active_bands.band_2.has_focus_pipeline_id = true;
  }
  if (m_payload.body.active_bands.band_1.tx_pipeline_id == makesdr_PipelineId_PIPELINE_NONE) {
    m_payload.body.active_bands.band_1.tx_pipeline_id = makesdr_PipelineId_PIPELINE_A;
    m_payload.body.active_bands.band_1.has_tx_pipeline_id = true;
  }
  if (m_payload.body.active_bands.band_2.tx_pipeline_id == makesdr_PipelineId_PIPELINE_NONE) {
    m_payload.body.active_bands.band_2.tx_pipeline_id = makesdr_PipelineId_PIPELINE_A;
    m_payload.body.active_bands.band_2.has_tx_pipeline_id = true;
  }
}

#ifdef USE_DOTTED_STRING_PATHS
ResolveDottedStringFunc
SplitBandDualIqRxTxSettings::resolveDottedStringFunc()
{
  return [](const char* dottedPath, FieldDescriptor& descriptor) -> ResultCode {
    return ::resolveDottedString(dottedPath, split_band_dual_iq_radio_fields, descriptor);
  };
}
#endif
