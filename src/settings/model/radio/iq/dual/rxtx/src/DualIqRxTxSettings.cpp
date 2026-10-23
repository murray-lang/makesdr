#include <settings/model/radio/iq/DualIqRxTxSettings.h>


#ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/update/DualIqRxTxTagLookup.h>
#include <settings/model/update/resolveDottedString.h>
#include <settings/model/update/ResolveDottedStringFunc.h>
#endif


DualIqRxTxSettings::DualIqRxTxSettings()
  : DualIqRxTxSettingsBaseType()
  // , m_transmitterSettings(m_payload.body.transmitter)
  , m_updater(static_cast<FieldUpdateSink*>(this))
{
  // m_payload.body.capabilities.rx = true;
  // m_payload.body.capabilities.has_tx = true;
  // m_payload.body.capabilities.has_iq = true;
  // m_payload.body.capabilities.iq.dual_pipeline = true;

  InitBandAndPipelineIdsWithDefaults();
}

DualIqRxTxSettings::DualIqRxTxSettings(const Payload& payload, bool complete)
  : DualIqRxTxSettingsBaseType(payload, complete)
  // , m_transmitterSettings(m_payload.body.transmitter)
  , m_updater(static_cast<FieldUpdateSink*>(this))
{
}

DualIqRxTxSettings::DualIqRxTxSettings(const DualIqRxTxSettings& other)
  : DualIqRxTxSettingsBaseType(other)
  // , m_transmitterSettings(m_payload.body.transmitter)
  , m_updater(static_cast<FieldUpdateSink*>(this))
{

}

DualIqRxTxSettings::DualIqRxTxSettings(DualIqRxTxSettings&& other) noexcept
  : DualIqRxTxSettingsBaseType(::move(other))
  // , m_transmitterSettings(m_payload.body.transmitter)
  , m_updater(static_cast<FieldUpdateSink*>(this))
{

}

DualIqRxTxSettings&
DualIqRxTxSettings::operator=(const DualIqRxTxSettings& other)
{
  if (this != &other) {
    replace(other.body(), true);
  }
  return *this;
}

void
DualIqRxTxSettings::InitBandAndPipelineIdsWithDefaults()
{
  if (m_payload.body.active_bands.focus_band.focus_pipeline_id == makesdr_PipelineId_PIPELINE_NONE) {
    m_payload.body.active_bands.focus_band.focus_pipeline_id = makesdr_PipelineId_PIPELINE_A;
    m_payload.body.active_bands.focus_band.has_focus_pipeline_id = true;
  }
  if (m_payload.body.active_bands.focus_band.tx_pipeline_id == makesdr_PipelineId_PIPELINE_NONE) {
    m_payload.body.active_bands.focus_band.tx_pipeline_id = makesdr_PipelineId_PIPELINE_A;
    m_payload.body.active_bands.focus_band.has_tx_pipeline_id = true;
  }
}

#ifdef USE_DOTTED_STRING_PATHS
ResolveDottedStringFunc
DualIqRxTxSettings::resolveDottedStringFunc()
{
  return [](const char* dottedPath, FieldDescriptor& descriptor) -> ResultCode {
    return ::resolveDottedString(dottedPath, dual_iq_rxtx_radio_fields, descriptor);
  };
}
#endif
