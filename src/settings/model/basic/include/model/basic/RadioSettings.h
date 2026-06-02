#pragma once

#include "../../../../core/include/model/core/SettingsBase.h"
#include "ReceiverSettings.h"
#include "TransmitterSettings.h"
#include "ResolveIndirection.h"

template <typename ActiveBandSettingsType>
class RadioSettings : public SettingsBase, public ResolveIndirection
{
public:
  RadioSettings()
    : m_rawSettings(RadioSettings_RadioSettingsPb_init_zero)
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

  ActiveBandSettingsType& activeBandSettings() { return m_activeBandSettings; }
  ReceiverSettings& receiverSettings() { return m_receiverSettings; }
  TransmitterSettings& transmitterSettings() { return m_transmitterSettings; }

  [[nodiscard]] const ActiveBandSettingsType& activeBandSettings() const { return m_activeBandSettings; }
  [[nodiscard]] const ReceiverSettings& receiverSettings() const { return m_receiverSettings; }
  [[nodiscard]] const TransmitterSettings& transmitterSettings() const { return m_transmitterSettings; }

  [[nodiscard]] bool getPtt() const { return m_rawSettings.ptt; }

  ResultCode resolveIndirection(
    const SettingFieldPath& indirectPath,
    uint32_t startingAtIndex,
    SettingFieldPath& resolvedPath
  ) override
  {
    if (startingAtIndex >= indirectPath.size()) {
      return ResultCode::ERR_SETTING_RESOLVE_INDIRECTION_PATH_INVALID;
    }
    switch (indirectPath[startingAtIndex]) {
    case RadioSettings_RadioSettingsPb_active_bands_tag:
      resolvedPath.emplace_back(RadioSettings_RadioSettingsPb_active_bands_tag);
      return m_activeBandSettings.resolveIndirection(indirectPath, startingAtIndex + 1, resolvedPath);
    }
    for (uint32_t i = startingAtIndex; i < indirectPath.size(); i++) {
      resolvedPath.emplace_back(indirectPath[i]);
    }
    return ResultCode::OK;

  }

protected:
  RadioSettings_RadioSettingsPb m_rawSettings;
  ActiveBandSettingsType m_activeBandSettings;
  ReceiverSettings m_receiverSettings;
  TransmitterSettings m_transmitterSettings;
};
