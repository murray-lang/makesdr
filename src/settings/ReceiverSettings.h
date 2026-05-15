#pragma once
#include "base/SettingsBase.h"
#include "pipeline/IqCorrectionSettings.h"
#include "RadioSettings.pb.h"

class ReceiverSettings : public SettingsBase
{
public:
  ReceiverSettings(RadioSettings_ReceiverSettings& raw)
    : SettingsBase(&RadioSettings_ReceiverSettings_msg)
    , m_rawSettings(raw)
    , m_iqCorrectionSettings(raw.iq_corrections)
  {}

  [[nodiscard]] bool hasIqCorrections() const { return m_rawSettings.has_iq_corrections; }

  IqCorrectionSettings& iqCorrectionSettings() { return m_iqCorrectionSettings; }
  [[nodiscard]] const IqCorrectionSettings& iqCorrectionSettings() const { return m_iqCorrectionSettings; }

protected:
  void* getMessage() override { return &m_rawSettings; }

  RadioSettings_ReceiverSettings& m_rawSettings;
  IqCorrectionSettings m_iqCorrectionSettings;
};
