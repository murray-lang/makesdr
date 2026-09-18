#pragma once
#include "DigitalOutputT.h"
#include <settings/model/radios/IRadioSettings.h>
#include <config/struct/BandSelectorConfig.h>

template <typename RadioSettingsT>
class GpioBandSelectorT : public DigitalOutputT<RadioSettingsT>
{
public:
  GpioBandSelectorT()
    : m_defaultOut(0)
    , m_currentOut(0)
  {
  }

  ~GpioBandSelectorT() override = default;

  GpioBandSelectorT(GpioBandSelectorT&&)  noexcept = default;
  GpioBandSelectorT& operator=(GpioBandSelectorT&&)  noexcept = default;

  ResultCode configure(const Config::BandSelector::Fields& config, ResolveDottedStringFunc resolver)
  {
    ResultCode rc = DigitalOutputT<RadioSettingsT>::configure(config, resolver);
    if (rc == ResultCode::OK) {
      m_defaultOut = config.defaultOut;
      m_bands = config.bands;
    }
    return rc;
  }

  ResultCode applySettings(RadioSettingsT& settings) override
  {
    if (settings.hasActiveBands()) {
      typename RadioSettingsT::ActiveBandSettings* activeBandSettings = settings.activeBands();
      const typename RadioSettingsT::BandSettings* bandSettings = activeBandSettings->txBand();
      if (bandSettings != nullptr && bandSettings->hasRfSettings()) {
        const BandRfSettings* rfSettings = bandSettings->rfSettings();
        if (rfSettings->hasFrequency()) {
          uint32_t frequency = rfSettings->frequency();
          uint32_t output = getBandOutput(frequency);
          applyOutput(output);
        }
      }
    }
    return ResultCode::OK;
  }
  ResultCode applyFieldUpdate(const FieldUpdate& setting) override
  {
    if (setting.path() == this->m_settingDescriptor.getPath()) {
      uint32_t frequency = get<uint32_t>(setting.value());
      uint32_t output = getBandOutput(frequency);
      if (output != m_currentOut) {
        return applyOutput(output);
      }
      return ResultCode::OK;
    }
    return ResultCode::ERR_SETTING_PATH_MISMATCH;
  }

  ResultCode ptt(bool on) override { return ResultCode::OK; };

protected:
  [[nodiscard]] uint32_t getBandOutput(uint32_t frequency) const
  {
    // Lookup the output for the given frequency
    for ( auto& band : m_bands) {
      if (frequency >= band->fromFrequency && frequency <= band->toFrequency) {
        return band->outValue;
      }
    }
    return m_defaultOut;
  }

  ResultCode applyOutput(uint32_t output)
  {
    m_currentOut = output;

    ResultCode rc = ResultCode::OK;
    int nextSignificantBit = 0;
    for (int i = 0; i < MAX_GPIO_LINES; i++) {
      GpioLineMask nextLine = this->m_lines & (1 << i);
      if (nextLine == 0) continue;
      GpioLineValue value = (output & (1 << nextSignificantBit)) != 0 ? 1 : 0;
      rc = this->m_linesRequest.lineWriter(nextLine, value);
      nextSignificantBit++;
      if (rc != ResultCode::OK) break;
    }
    return rc;
  }

  // FieldPath m_frequencyFieldPath;
  // FieldPath m_offsetFieldPath;
  uint32_t m_defaultOut;
  uint32_t m_currentOut;
  Config::BandSelector::BandsVector m_bands{};

};

