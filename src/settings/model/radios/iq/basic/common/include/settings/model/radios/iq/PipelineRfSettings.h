#pragma once

#include <settings/model/radios/BandRfSettings.h>
#include <settings/model/proto/RadioSettings.pb.h>

class PipelineRfSettings : public IApplyBandDefaults
{
public:

  using Proto = makesdr_PipelineRfSettingsPb;

  PipelineRfSettings(Proto& raw)
  : m_rawSettings(raw)
  {
  }

  [[nodiscard]] bool hasFrequency() const { return m_rawSettings.has_frequency; }
  [[nodiscard]] int64_t frequency() const { return m_rawSettings.frequency.value; }
  void setFrequency(int64_t frequency) { m_rawSettings.frequency.value = frequency; }

  [[nodiscard]] bool hasMaxPositiveOffset() const { return m_rawSettings.has_max_positive_offset; }
  [[nodiscard]] bool hasMaxNegativeOffset() const { return m_rawSettings.has_max_negative_offset; }

  [[nodiscard]] int32_t maxPositiveOffset() const { return m_rawSettings.max_positive_offset; }
  [[nodiscard]] int32_t maxNegativeOffset() const { return m_rawSettings.max_negative_offset; }

  ResultCode applyBandDefaults(const Band& band, const BandCategoryList* bands, const ModeList* modes) override
  {
    m_rawSettings.has_frequency = true;
    m_rawSettings.frequency.value = band.landingFrequency();
    m_rawSettings.frequency.coarse_delta = band.defaultCoarseStep();
    m_rawSettings.frequency.fine_delta = band.defaultFineStep();
    return ResultCode::OK;
  }

  void setNyquistLimits(int32_t maxNegative, int32_t maxPositive)
  {
    setMaxPositiveOffset(maxPositive);
    setMaxNegativeOffset(maxNegative);
  }

  void setMaxPositiveOffset(int32_t offset)
  {
    m_rawSettings.max_positive_offset = offset;
    m_rawSettings.has_max_positive_offset = true;
  }

  void setMaxNegativeOffset(int32_t offset)
  {
    m_rawSettings.max_negative_offset = offset;
    m_rawSettings.has_max_negative_offset = true;
  }

  bool clampToNyquistLimits(const BandRfSettings& bandRfSettings, int32_t maxNegative, int32_t maxPositive)
  {
    m_rawSettings.has_max_negative_offset = true;
    m_rawSettings.has_max_positive_offset = true;
    m_rawSettings.max_negative_offset = maxNegative;
    m_rawSettings.max_positive_offset = maxPositive;

    int64_t centreFreq = bandRfSettings.frequency();
    int64_t vfo = frequency();
    auto offset = static_cast<int32_t>(centreFreq - vfo);
    if (offset > maxPositive) {
      setFrequency(centreFreq + maxPositive);
      m_rawSettings.has_frequency = true;
      return true;
    } else if (offset < maxNegative) {
      setFrequency(centreFreq + maxNegative);
      m_rawSettings.has_frequency = true;
      return true;
    }
    return false;
  }

protected:
  Proto& m_rawSettings;
};
