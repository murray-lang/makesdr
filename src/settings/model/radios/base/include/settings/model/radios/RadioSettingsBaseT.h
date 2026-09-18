#pragma once
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/proto/ProtobufIo.h>
#include <settings/model/data/radio/RadioLookup.h>
#include <settings/model/message/FieldUpdateSink.h>
#include <EventId.h>
#include "IRadioSettingsWithUpdater.h"
#include <settings/model/message/MessageT.h>


#define COMMON_ACTIVE_BANDS_TAG 1

template<
  typename SettingsPbType,
  const pb_msgdesc_t* descriptor,
  typename PayloadPbType,
  makesdr_RadioPayloadType payloadTypeEnum,
  int payloadSize,
  typename ActiveBandSettingsClass,
  typename CacheClass
>
class RadioSettingsBaseT :
    public MessageT<SettingsPbType, descriptor, PayloadPbType, payloadTypeEnum, payloadSize>,
    public IRadioSettingsWithUpdater
{
public:

  using Proto = SettingsPbType;
  using Payload = PayloadPbType;
  using ActiveBandSettings = ActiveBandSettingsClass;
  using BandSettings = ActiveBandSettingsClass::BandSettings;
  using Cache = CacheClass;

  using MessageBase = MessageT<SettingsPbType, descriptor, PayloadPbType, payloadTypeEnum, payloadSize>;

  RadioSettingsBaseT()
  : MessageBase()
  , m_activeBandSettings(this->m_payload.body.active_bands)
  , m_receiver(this->m_payload.body.receiver)
  , m_bands(nullptr)
  , m_modes(nullptr)
  , m_cache(nullptr)
  {
  }

  RadioSettingsBaseT(const RadioSettingsBaseT& other) noexcept
    : MessageBase(other)
    , m_activeBandSettings(this->m_payload.body.active_bands)
    , m_receiver(this->m_payload.body.receiver)
    , m_bands(other.m_bands)
    , m_modes(other.m_modes)
    , m_cache(other.m_cache)
  {
  }

  RadioSettingsBaseT(RadioSettingsBaseT&& other) noexcept
    : MessageBase(other)
    , m_activeBandSettings(this->m_payload.body.active_bands)
    , m_receiver(this->m_payload.body.receiver)
    , m_bands(other.m_bands)
    , m_modes(other.m_modes)
    , m_cache(other.m_cache)
  {
  }

  RadioSettingsBaseT(const PayloadPbType& payload, bool complete = true)
  : MessageBase(payload, complete)
  , m_activeBandSettings(this->m_payload.body.active_bands)
  , m_receiver(this->m_payload.body.receiver)
  , m_bands(nullptr)
  , m_modes(nullptr)
  , m_cache(nullptr)
  {
  }

  // RadioSettingsBaseT& operator=(const RadioSettingsBaseT& other) noexcept
  // {
  //   if (this != &other) {
  //     MessageBase::operator=(other);
  //     m_activeBandSettings = ActiveBandSettingsClass(this->m_payload.body.active_bands);
  //     m_receiver = ReceiverSettings(this->m_payload.body.receiver);
  //     m_lookup = other.m_lookup;
  //     m_cache = other.m_cache;
  //   }
  //   return *this;
  // }
  //
  // RadioSettingsBaseT& operator=(RadioSettingsBaseT&& other) noexcept
  // {
  //   if (this != &other) {
  //     MessageBase::operator=(::move(other));
  //     m_activeBandSettings = ActiveBandSettingsClass(this->m_payload.body.active_bands);
  //     m_receiver = ReceiverSettings(this->m_payload.body.receiver);
  //     m_lookup = ::move(other.m_lookup);
  //     m_cache = other.m_cache;
  //   }
  //   return *this;
  // }

  ~RadioSettingsBaseT() override = default;



  ResultCode applyFieldUpdate(const FieldUpdate &settingUpdate) override
  {
    ResultCode rc = ResultCode::OK;
    if (settingUpdate.isIndirect()) {
      rc = updateIndirectField(settingUpdate);
    } else {
      rc = MessageBase::applyFieldUpdate(settingUpdate);
    }
    if (rc == ResultCode::OK && settingUpdate.needsAutoComplete()) {
      rc = autoComplete(settingUpdate.descriptor());
    }
    return rc;
  }

  ResultCode updateIndirectField(const FieldUpdate &settingUpdate)
  {
    if (settingUpdate.descriptor().getPath().at(0) == COMMON_ACTIVE_BANDS_TAG) {
      return m_activeBandSettings.updateIndirectField(settingUpdate, 1);
    }
    return ResultCode::ERR_SETTING_INDIRECT_PATH_INVALID;
  }

  void setBands(const BandCategoryList* bands) { m_bands = bands; }
  void setModes(const ModeList* modes) { m_modes = modes; }
  void setCache(CacheClass* cache) { m_cache = cache; }

  [[nodiscard]] const Band * getFocusBand() const override { return m_activeBandSettings.getFocusBand(); }
  [[nodiscard]] const Mode * getFocusMode() const override { return m_activeBandSettings.getFocusMode(); }

  [[nodiscard]] bool hasActiveBands() const override { return this->m_payload.body.has_active_bands; }
  ActiveBandSettings* activeBands() { return &m_activeBandSettings; }
  [[nodiscard]] const ActiveBandSettings* activeBands() const { return &m_activeBandSettings; }

  [[nodiscard]] bool hasReceiver() const override { return this->m_payload.body.has_receiver; }
  ReceiverSettings* receiver() override { return &m_receiver; }
  [[nodiscard]] const ReceiverSettings* receiver() const override { return &m_receiver; }

  [[nodiscard]] bool hasPtt() const override { return this->m_payload.body.has_ptt; }
  [[nodiscard]] bool ptt() const override { return this->m_payload.body.ptt; }

  ResultCode autoComplete()
  {
    return m_activeBandSettings.autoComplete(m_bands, m_modes, m_cache);
  }

  ResultCode autoComplete(const FieldDescriptor& setting)
  {
    const FieldPath& path = setting.getPath();
    if (path[0] == COMMON_ACTIVE_BANDS_TAG) {
      return m_activeBandSettings.autoComplete(setting, 1, m_bands, m_modes, m_cache);
    }
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
  }

protected:
  ActiveBandSettingsClass m_activeBandSettings;
  ReceiverSettings m_receiver;

  const BandCategoryList* m_bands;
  const ModeList* m_modes;
  CacheClass*  m_cache;
};
