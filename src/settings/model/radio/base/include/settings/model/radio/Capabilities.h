#pragma once
#include <settings/model/proto/RadioSettings.pb.h>

#ifdef USE_ETL
#include <etl/span.h>

using FrequencyRangeSpan = etl::span<makesdr_FrequencyRangePb, MAX_FREQUENCY_RANGES>;
#else
#include <span>
using FrequencyRangeSpan = std::span<makesdr_FrequencyRangePb>;
#endif

class Capabilities
{
public:
  using Proto = makesdr_CapabilitiesPb;

  Capabilities(Proto& rawSettings)
    : m_rawSettings(rawSettings)
    , m_frequencyRanges(m_rawSettings.frequency_ranges, m_rawSettings.frequency_ranges_count)
  {
  }

  [[nodiscard]] FrequencyRangeSpan& getFrequencyRanges()
  {
    return m_frequencyRanges;
  }

  void setFrequencyRange(const FrequencyRangeSpan& frequencyRanges)
  {
    for (int i = 0; i < frequencyRanges.size(); i++) {
      m_rawSettings.frequency_ranges[i] = frequencyRanges[i];
    }
    m_rawSettings.frequency_ranges_count = frequencyRanges.size();
  }

  bool getIqSampleRate(uint32_t* sampleRate) const
  {
    if (m_rawSettings.has_iq && m_rawSettings.iq.has_sample_rate) {
      *sampleRate = m_rawSettings.iq.sample_rate;
      return true;
    }
    return false;
  }

  void setIqSampleRate(uint32_t sampleRate) const
  {
    m_rawSettings.iq.sample_rate = sampleRate;
    m_rawSettings.iq.has_sample_rate = true;
    m_rawSettings.has_iq = true;
  }

private:
  Proto& m_rawSettings;
  FrequencyRangeSpan m_frequencyRanges;
};