#pragma once

#include "IApplyBandDefaults.h"
#include <settings/model/proto/RadioSettings.pb.h>

class BandRfSettings : public IApplyBandDefaults
{
public:
  using Proto = makesdr_BandRfSettingsPb;
  BandRfSettings(Proto& raw) : m_rawSettings(raw) {}

  [[nodiscard]] bool hasFrequency() const { return m_rawSettings.has_frequency; }
  [[nodiscard]] int64_t frequency() const { return m_rawSettings.frequency.value; }

  [[nodiscard]] bool hasGain() const  { return m_rawSettings.has_gain; }
  [[nodiscard]] float gain() const { return m_rawSettings.gain.value;}

  ResultCode applyBandDefaults(const Band& band, const BandCategoryList* bands, const ModeList* modes) override
  {
    m_rawSettings.has_frequency = true;
    m_rawSettings.frequency.value = band.landingFrequency();
    m_rawSettings.frequency.coarse_delta = band.defaultCoarseStep();
    m_rawSettings.frequency.fine_delta = band.defaultFineStep();
    return ResultCode::OK;
  }

protected:
  Proto& m_rawSettings;
};