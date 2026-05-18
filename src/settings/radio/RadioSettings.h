#pragma once

#include "../base/SettingsBase.h"
#include "../band/ActiveBandSettings.h"
#include "ReceiverSettings.h"
#include "TransmitterSettings.h"
#include "../util/DefaultRadioSettings.h"

class RadioSettings : public SettingsBase
{
public:
  RadioSettings()
    : m_rawSettings(defaultRadioSettingsPb)
    , m_activeBandSettings(m_rawSettings.active_bands)
    , m_receiverSettings(m_rawSettings.receiver)
    , m_transmitterSettings(m_rawSettings.transmitter)
  {}

  RadioSettings(const RadioSettings_RadioSettingsPb& raw)
    : m_rawSettings(raw)
    , m_activeBandSettings(m_rawSettings.active_bands)
    , m_receiverSettings(m_rawSettings.receiver)
    , m_transmitterSettings(m_rawSettings.transmitter)
  {
  }

  RadioSettings_RadioSettingsPb& rawSettings() { return m_rawSettings; }
  [[nodiscard]] const RadioSettings_RadioSettingsPb& rawSettings() const { return m_rawSettings; }

  [[nodiscard]] bool hasReceiver() const { return m_rawSettings.has_receiver; }
  [[nodiscard]] bool hasTransmitter() const { return m_rawSettings.has_transmitter; }
  [[nodiscard]] bool hasActiveBands() const { return m_rawSettings.has_active_bands; }
  [[nodiscard]] bool hasPtt() const { return m_rawSettings.has_ptt; }

  ActiveBandSettings& activeBandSettings() { return m_activeBandSettings; }
  ReceiverSettings& receiverSettings() { return m_receiverSettings; }
  TransmitterSettings& transmitterSettings() { return m_transmitterSettings; }

  [[nodiscard]] const ActiveBandSettings& activeBandSettings() const { return m_activeBandSettings; }
  [[nodiscard]] const ReceiverSettings& receiverSettings() const { return m_receiverSettings; }
  [[nodiscard]] const TransmitterSettings& transmitterSettings() const { return m_transmitterSettings; }

  [[nodiscard]] bool getPtt() const { return m_rawSettings.ptt; }

  ResultCode readProtobuf(const uint8_t *buffer, size_t msg_length);

protected:
  RadioSettings_RadioSettingsPb m_rawSettings;
  ActiveBandSettings m_activeBandSettings;
  ReceiverSettings m_receiverSettings;
  TransmitterSettings m_transmitterSettings;
};
