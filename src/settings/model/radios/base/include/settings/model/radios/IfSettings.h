#pragma once


class IfSettings
{
public:
  IfSettings(makesdr_IfSettingsPb& rawSettings)
    : m_rawSettings(rawSettings)
  {}

  [[nodiscard]] bool hasBandwidth() const { return m_rawSettings.has_bandwidth; }
  [[nodiscard]] uint32_t bandwidth() const { return m_rawSettings.bandwidth; }

  [[nodiscard]] bool hasGain() const { return m_rawSettings.has_gain; }
  [[nodiscard]] float gain() const { return m_rawSettings.gain.value; }

protected:
  makesdr_IfSettingsPb& m_rawSettings;
};
