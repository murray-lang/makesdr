#include <gpio/service/Gpio.h>
#include "settings/model/core/RadioSettings.h"
#include "settings/control/digital/GpioBandSelector.h"


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
GpioBandSelector::applySettings(const RadioSettings& settings)
{
  if (settings.hasActiveBands()) {
    const makesdr_RxPipelineSettingsPb* txPipelineSettings = settings.getTxPipelineSettings();
    if (txPipelineSettings != nullptr) {
      if (txPipelineSettings->base.has_rf) {
        const makesdr_RfSettingsPb& rfSettings = txPipelineSettings->base.rf;
        if (rfSettings.has_vfo) {
          uint32_t frequency = rfSettings.vfo.value;
          uint32_t output = getBandOutput(frequency);
          applyOutput(output);
        }
      }
    }
  }
  return ResultCode::OK;
}

ResultCode
GpioBandSelector::applySettingUpdate(const SettingUpdate& setting)
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
  m_currentOut = output;

  ResultCode rc = ResultCode::OK;
  int nextSignificantBit = 0;
  for (int i = 0; i < MAX_GPIO_LINES; i++) {
    GpioLineMask nextLine = m_lines & (1 << i);
    if (nextLine == 0) continue;
    GpioLineValue value = (output & (1 << nextSignificantBit)) != 0 ? 1 : 0;
    rc = m_linesRequest.lineWriter(nextLine, value);
    nextSignificantBit++;
    if (rc != ResultCode::OK) break;
  }
  return rc;
}
