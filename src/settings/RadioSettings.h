#pragma once

#include "base/SettingsBase.h"
#include "ActiveBandSettings.h"
#include "ReceiverSettings.h"
#include "TransmitterSettings.h"

class RadioSettings : public SettingsBase
{
public:
  RadioSettings()
    : SettingsBase(&RadioSettings_RadioSettingsPb_msg)
    , m_rawSettings(RadioSettings_RadioSettingsPb_init_zero)
    , m_activeBandSettings(m_rawSettings.active_bands)
    , m_receiverSettings(m_rawSettings.receiver)
    , m_transmitterSettings(m_rawSettings.transmitter)
  {}

  RadioSettings(RadioSettings_RadioSettingsPb& raw)
    : SettingsBase(&RadioSettings_RadioSettingsPb_msg)
    , m_rawSettings(raw)
    , m_activeBandSettings(m_rawSettings.active_bands)
    , m_receiverSettings(m_rawSettings.receiver)
    , m_transmitterSettings(m_rawSettings.transmitter)
  {
  }

  RadioSettings_RadioSettingsPb& rawSettings() { return m_rawSettings; }
  [[nodiscard]] const RadioSettings_RadioSettingsPb& rawSettings() const { return m_rawSettings; }

  ActiveBandSettings& activeBandSettings() { return m_activeBandSettings; }
  ReceiverSettings& receiverSettings() { return m_receiverSettings; }
  TransmitterSettings& transmitterSettings() { return m_transmitterSettings; }

  [[nodiscard]] const ActiveBandSettings& activeBandSettings() const { return m_activeBandSettings; }
  [[nodiscard]] const ReceiverSettings& receiverSettings() const { return m_receiverSettings; }
  [[nodiscard]] const TransmitterSettings& transmitterSettings() const { return m_transmitterSettings; }

  [[nodiscard]] bool getPtt() const { return m_rawSettings.ptt; }

protected:
  RadioSettings_RadioSettingsPb m_rawSettings;
  ActiveBandSettings m_activeBandSettings;
  ReceiverSettings m_receiverSettings;
  TransmitterSettings m_transmitterSettings;
};
