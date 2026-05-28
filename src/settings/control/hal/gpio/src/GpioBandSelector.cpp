#include "gpio/base/Gpio.h"
#include "model/MutableRadioSettings.h"
#include "gpio/GpioBandSelector.h"


GpioBandSelector::GpioBandSelector() :
  m_defaultOut(0),
  m_currentOut(0)
{
}

ResultCode
GpioBandSelector::configure(const Config::BandSelector::Fields& config)
{
  ResultCode rc = DigitalOutput::configure(config);
  if (rc == ResultCode::OK) {
    m_defaultOut = config.defaultOut;
    m_bands = config.bands;
  }
  return rc;
}

ResultCode
GpioBandSelector::applySettings(const BasicRadioSettings& settings)
{
  if (settings.hasActiveBands()) {
    const BasicActiveBandSettings& activeBandSettings = settings.activeBandSettings();
    if (activeBandSettings.hasTxBandId()) {
      const BasicBandSettings* pBandSettings = activeBandSettings.getTxBandSettings();
      if (pBandSettings != nullptr && pBandSettings->hasTxPipeline()) {
        const PipelineSettings& pipelineSettings = pBandSettings->txPipelineSettings().base();
        if (pipelineSettings.hasRf()) {
          const RfSettings& rfSettings = pipelineSettings.rfSettings();
          if (rfSettings.hasVfoFrequency()) {
            uint32_t frequency = rfSettings.vfoFrequency().value();
            uint32_t output = getBandOutput(frequency);
            applyOutput(output);
          }
        }
      }
    }
  }
  return ResultCode::OK;
}

ResultCode
GpioBandSelector::applySettingFieldUpdate(const SettingFieldUpdate& setting)
{
  if (setting.path() == m_settingPath) {
    uint32_t frequency = get<uint32_t>(setting.value());
    uint32_t output = getBandOutput(frequency);
    if (output != m_currentOut) {
      return applyOutput(output);
    }
    return ResultCode::OK;
  }
  return ResultCode::ERR_SETTING_PATH_MISMATCH;
}

uint32_t
GpioBandSelector::getBandOutput(uint32_t frequency) const
{
  // Lookup the output for the given frequency
  for ( auto& band : m_bands) {
    if (frequency >= band->fromFrequency && frequency <= band->toFrequency) {
      return band->outValue;
    }
  }
  return m_defaultOut;
}

ResultCode
GpioBandSelector::applyOutput(uint32_t output)
{
  if (!m_linesRequest) {
    return ResultCode::ERR_SETTING_DIGITAL_OUTPUT_NO_LINE_REQUEST;
  }
  m_currentOut = output;
  GpioOutputValuesVector values(m_lines.size(), false);
  for (size_t i = 0; i < m_lines.size(); ++i) {
    values.at(i) = ((output >> i) & 0x1u) != 0;
  }
  return m_linesRequest->setLineValues(m_lines, values);
}
