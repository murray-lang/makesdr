#pragma once
#include "base/SettingsBase.h"
#include "IqCorrectionSettings.h"
#include "RadioSettings.pb.h"

class ReceiverSettings : public SettingsBase
{
public:
  ReceiverSettings(RadioSettings_ReceiverSettings& raw)
    : SettingsBase(&RadioSettings_ReceiverSettings_msg)
    , m_rawSettings(raw)
    , m_iqCorrectionSettings(raw.iq_corrections)
  {}

  IqCorrectionSettings& iqCorrectionSettings() { return m_iqCorrectionSettings; }
  [[nodiscard]] const IqCorrectionSettings& iqCorrectionSettings() const { return m_iqCorrectionSettings; }

protected:
  RadioSettings_ReceiverSettings& m_rawSettings;
  IqCorrectionSettings m_iqCorrectionSettings;
};
