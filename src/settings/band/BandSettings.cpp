#include "BandSettings.h"


BandSettings::BandSettings(RadioSettings_BandSettingsPb& raw)
    : m_rawSettings(raw)
    , m_pipelineA(raw.pipeline_a)
    , m_pipelineB(raw.pipeline_b)
    , m_txPipeline(raw.tx_pipeline)
{
  setBandOrRequestVariant(m_rawSettings);
}

// BandSettings(BandSettings&& rhs) noexcept;

BandSettings&
BandSettings::operator=(const BandSettings& rhs) noexcept
{
  if (this != &rhs) {
    operator=(rhs.m_rawSettings);
  }
  return *this;
}

BandSettings&
BandSettings::operator=(const RadioSettings_BandSettingsPb& rhs) noexcept
{
  m_rawSettings = rhs;
  setBandOrRequestVariant(m_rawSettings);
  return *this;
}

[[nodiscard]] const StringRef*
BandSettings::bandName() const
{
  const StringRef* request = etl::get_if<StringRef>(&m_bandOrRequest);
  if (request != nullptr) return request;
  const Band* band = etl::get_if<Band>(&m_bandOrRequest);
  if (band != nullptr) return &band->name();
  return nullptr;
}

void
BandSettings::setBandOrRequestVariant(RadioSettings_BandSettingsPb& raw)
{
  if (raw.which_band_or_request == RadioSettings_BandSettingsPb_band_request_tag) {
    m_bandOrRequest.emplace<StringRef>(
      raw.band_or_request.band_request,
      raw.band_or_request.band_request,
      sizeof(raw.band_or_request.band_request)
    );
  } else if (raw.which_band_or_request == RadioSettings_BandSettingsPb_band_tag) {
    m_bandOrRequest.emplace<Band>(raw.band_or_request.band);
  }
}

