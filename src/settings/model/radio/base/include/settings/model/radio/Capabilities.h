#pragma once
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/message/PayloadType.h>
#include <config/struct/CapabilitiesConfig.h>

// #ifdef USE_ETL
// #include <etl/span.h>
//
// using FrequencyRangeSpan = etl::span<makesdr_FrequencyRangePb, MAX_FREQUENCY_RANGES>;
// #else
// #include <span>
// using FrequencyRangeSpan = std::span<makesdr_FrequencyRangePb>;
// #endif

class Capabilities
{
public:
  using Proto = makesdr_CapabilitiesPb;

  using FrequencyRange = makesdr_FrequencyRangePb;

  Capabilities(Proto& rawSettings)
    : m_rawSettings(rawSettings)
    // , m_frequencyRanges(m_rawSettings.frequency_ranges, m_rawSettings.frequency_ranges_count)
  {
  }

  // The settings class the radio uses. The client instantiates the same class
  // and reads its Traits to decide what to display.
  [[nodiscard]] PayloadType getSettingsType() const
  {
    return static_cast<PayloadType>(m_rawSettings.settings_type);
  }

  void setSettingsType(PayloadType settingsType)
  {
    m_rawSettings.settings_type = static_cast<makesdr_RadioPayloadType>(settingsType);
  }

  [[nodiscard]] uint32_t getFrequencyRanges(FrequencyRange** ranges)
  {
    *ranges = m_rawSettings.frequency_ranges;
    return m_rawSettings.frequency_ranges_count;
  }

  void setFrequencyRanges(const FrequencyRangeConfigVector& frequencyRanges)
  {
    for (int i = 0; i < frequencyRanges.size(); i++) {
      m_rawSettings.frequency_ranges[i].min = frequencyRanges[i].min;
      m_rawSettings.frequency_ranges[i].max = frequencyRanges[i].max;
    }
    m_rawSettings.frequency_ranges_count = frequencyRanges.size();
  }

  // void setFrequencyRanges(const FrequencyRangeSpan& frequencyRanges)
  // {
  //   for (int i = 0; i < frequencyRanges.size(); i++) {
  //     m_rawSettings.frequency_ranges[i] = frequencyRanges[i];
  //   }
  //   m_rawSettings.frequency_ranges_count = frequencyRanges.size();
  // }

  bool getIqSampleRate(uint32_t* sampleRate) const
  {
    if (m_rawSettings.has_sample_rate) {
      *sampleRate = m_rawSettings.sample_rate;
      return true;
    }
    return false;
  }

  void setIqSampleRate(uint32_t sampleRate) const
  {
    m_rawSettings.sample_rate = sampleRate;
    m_rawSettings.has_sample_rate = true;
  }

private:
  Proto& m_rawSettings;
  // FrequencyRangeSpan m_frequencyRanges;
};
