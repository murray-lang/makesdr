#pragma once

#include "base/SettingsBase.h"
#include "band/ActiveBandSettings.h"
#include "ReceiverSettings.h"
#include "TransmitterSettings.h"
#include "util/DefaultRadioSettings.h"

class SimpleRadioSettings : public SettingsBase
{
public:
  SimpleRadioSettings()
    : SettingsBase(&RadioSettings_RadioSettingsPb_msg)
    , m_rawSettings(defaultRadioSettingsPb)
    , m_activeBandSettings(m_rawSettings.active_bands)
    , m_receiverSettings(m_rawSettings.receiver)
    , m_transmitterSettings(m_rawSettings.transmitter)
  {}

  SimpleRadioSettings(const RadioSettings_RadioSettingsPb& raw)
    : SettingsBase(&RadioSettings_RadioSettingsPb_msg)
    , m_rawSettings(raw)
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


  ResultCode getFocusBandId(SplitBandId* pBandId);
  ResultCode getTxBandId(SplitBandId* pBandId);
  ResultCode getRxBandId(SplitBandId* pBandId);
  ResultCode getFocusPipelineId(SplitBandId bandId, PipelineId* pPipelineId);
  ResultCode isSplitBands(bool* pIsSplitBands);

protected:
  void* getMessage() override { return &m_rawSettings; }

  ResultCode getBandId(const SettingFieldPath& path, SplitBandId* pBandId);
  ResultCode getPipelineId(const SettingFieldPath& path, PipelineId* pPipelineId);
  ResultCode getBool(const SettingFieldPath& path, bool* pBool);

  RadioSettings_RadioSettingsPb m_rawSettings;
  ActiveBandSettings m_activeBandSettings;
  ReceiverSettings m_receiverSettings;
  TransmitterSettings m_transmitterSettings;
};
