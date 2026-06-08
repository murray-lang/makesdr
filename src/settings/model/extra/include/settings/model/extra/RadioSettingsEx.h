#pragma once

#include "settings/model/core/SettingsBase.h"
#include "ReceiverSettings.h"
#include "TransmitterSettings.h"
#include "ResolveIndirection.h"
#include "AutoComplete.h"
#include "ActiveBandSettings.h"
#include "settings/model/core/RadioSettings.h"
#include "settings/model/core/ProtobufIo.h"
#include "BandSettingsCache.h"

class RadioSettingsEx : public RadioSettings, public ResolveIndirection, public AutoComplete
{
public:
  RadioSettingsEx(RadioSettings_RadioSettingsPb& raw, const RadioMeta& meta)
    : RadioSettings(raw, meta.raw())
    , m_activeBandSettings(m_rawSettings.active_bands)
    , m_receiverSettings(m_rawSettings.receiver)
    , m_transmitterSettings(m_rawSettings.transmitter)
    , m_meta(meta)
  {
    m_activeBandSettings.setCategories(&m_meta);
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

  ResultCode resolveIndirection(
    const SettingFieldPath& indirectPath,
    uint32_t startingAtIndex,
    SettingFieldPath& resolvedPath
  ) override
  {
    if (startingAtIndex >= indirectPath.size()) {
      return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
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

  ResultCode readProtobuf(const uint8_t *buffer, size_t msg_length) {
    return ProtobufIo::readProtobuf<RadioSettings_RadioSettingsPb>(
      buffer,
      msg_length,
      RadioSettings_RadioSettingsPb_fields,
      RadioSettings_RadioSettingsPb_init_zero,
      m_rawSettings
    );
  }

  static ResultCode resolveDottedPath(
    const char *dottedPath,
    SettingFieldPath &path,
    bool* isIndirectOut,
    AutoCompleteTrigger* triggerOut
  );

  ResultCode applyUpdate(const SettingFieldUpdate& update);
  ResultCode autoComplete(const SettingFieldPath& path, uint32_t startingAtIndex, AutoCompleteTrigger trigger) override;
  ResultCode autoComplete() override;

protected:
  ActiveBandSettings m_activeBandSettings;
  ReceiverSettings m_receiverSettings;
  TransmitterSettings m_transmitterSettings;

  RadioMeta m_meta;
  BandSettingsCache m_bandSettingsCache;
};
