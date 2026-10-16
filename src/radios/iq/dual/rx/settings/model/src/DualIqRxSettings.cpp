#include "radios/settings/model/DualIqRxSettings.h"
#ifdef USE_DOTTED_STRING_PATHS
#include <radios/settings/update/DualIqRxTagLookup.h>
#include <settings/model/update/resolveDottedString.h>
#endif

DualIqRxSettings::DualIqRxSettings()
  : DualIqRxSettingsBaseType()
  , m_updater(static_cast<FieldUpdateSink*>(this))
{
   InitBandAndPipelineIdsWithDefaults();
}



DualIqRxSettings::DualIqRxSettings(const Payload& payload, bool complete)
  : DualIqRxSettingsBaseType(payload, complete)
  , m_updater(static_cast<FieldUpdateSink*>(this))
{
}

DualIqRxSettings::DualIqRxSettings(const DualIqRxSettings& other)
  : DualIqRxSettingsBaseType(other)
  , m_updater(static_cast<FieldUpdateSink*>(this))
{

}

DualIqRxSettings::DualIqRxSettings(DualIqRxSettings&& other) noexcept
  : DualIqRxSettingsBaseType(::move(other))
  , m_updater(static_cast<FieldUpdateSink*>(this))
{

}

DualIqRxSettings&
DualIqRxSettings::operator=(const DualIqRxSettings& other)
{
  if (this != &other) {
    replace(other.body(), true);
  }
  return *this;
}
void
DualIqRxSettings::InitBandAndPipelineIdsWithDefaults()
{
  if (m_payload.body.active_bands.focus_band.focus_pipeline_id == makesdr_PipelineId_PIPELINE_NONE) {
    m_payload.body.active_bands.focus_band.focus_pipeline_id = makesdr_PipelineId_PIPELINE_A;
    m_payload.body.active_bands.focus_band.has_focus_pipeline_id = true;
  }
}

#ifdef USE_DOTTED_STRING_PATHS
ResolveDottedStringFunc
DualIqRxSettings::resolveDottedStringFunc()
{
  return [](const char* dottedPath, FieldDescriptor& descriptor) -> ResultCode {
    return ::resolveDottedString(dottedPath, dual_iq_rx_radio_fields, descriptor);
  };
}
#endif
