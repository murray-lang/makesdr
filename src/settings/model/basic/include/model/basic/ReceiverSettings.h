#pragma once
#include "../../../../core/include/model/core/SettingsBase.h"
#include "IqCorrectionSettings.h"
#include "model/proto/RadioSettings.pb.h"

class ReceiverSettings : public SettingsBase
{
public:
  ReceiverSettings(RadioSettings_ReceiverSettings& raw)
    : m_rawSettings(raw)
    , m_iqCorrectionSettings(raw.iq_corrections)
  {}

  [[nodiscard]] bool hasIqCorrections() const { return m_rawSettings.has_iq_corrections; }

  IqCorrectionSettings& iqCorrectionSettings() { return m_iqCorrectionSettings; }
  [[nodiscard]] const IqCorrectionSettings& iqCorrectionSettings() const { return m_iqCorrectionSettings; }

protected:
  RadioSettings_ReceiverSettings& m_rawSettings;
  IqCorrectionSettings m_iqCorrectionSettings;
};
