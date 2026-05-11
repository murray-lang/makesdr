#pragma once
#include "base/SettingsBase.h"
#include "IqCorrectionSettings.h"
#include "MicrophoneSettings.h"


class TransmitterSettings : public SettingsBase
{
public:
  TransmitterSettings(RadioSettings_TransmitterSettingsPb& raw)
    : SettingsBase(&RadioSettings_TransmitterSettingsPb_msg)
    , m_rawSettings(raw)
    , m_iqCorrectionSettings(raw.iq_corrections)
    , m_microphoneSettings(raw.mic)
  {}
  IqCorrectionSettings& iqCorrectionSettings() { return m_iqCorrectionSettings; }
  MicrophoneSettings& microphoneSettings() { return m_microphoneSettings; }

  [[nodiscard]] const IqCorrectionSettings& iqCorrectionSettings() const { return m_iqCorrectionSettings; }
  [[nodiscard]] const MicrophoneSettings& microphoneSettings() const { return m_microphoneSettings; }
protected:
  RadioSettings_TransmitterSettingsPb& m_rawSettings;
  IqCorrectionSettings m_iqCorrectionSettings;
  MicrophoneSettings m_microphoneSettings;
};
