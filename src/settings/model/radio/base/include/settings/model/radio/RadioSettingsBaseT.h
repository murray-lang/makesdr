#pragma once
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/proto/ProtobufIo.h>
#include <settings/model/meta/radio/RadioLookup.h>
#include <settings/model/update/FieldUpdateSink.h>
#include <EventId.h>
#include "IRadioSettingsWithUpdater.h"
#include <settings/model/message/MessageT.h>
#include <settings/model/update/MessageTraverser.h>

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
    public IRadioSettingsWithUpdater,
    public FieldUpdateSink
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
  , m_deemComplete(false)
  , m_traverser(&MessageBase::body(), descriptor)
  , m_activeBandSettings(this->m_payload.body.active_bands)
  , m_receiver(this->m_payload.body.receiver)
  , m_bands(nullptr)
  , m_modes(nullptr)
  , m_cache(nullptr)
  {
  }

  RadioSettingsBaseT(const RadioSettingsBaseT& other) noexcept
    : MessageBase(other)
    , m_deemComplete(other.m_deemComplete)
    , m_traverser(&MessageBase::body(), descriptor)
    , m_activeBandSettings(this->m_payload.body.active_bands)
    , m_receiver(this->m_payload.body.receiver)
    , m_bands(other.m_bands)
    , m_modes(other.m_modes)
    , m_cache(other.m_cache)
  {
  }

  RadioSettingsBaseT(RadioSettingsBaseT&& other) noexcept
    : MessageBase(other)
    , m_deemComplete(other.m_deemComplete)
    , m_traverser(&MessageBase::body(), descriptor)
    , m_activeBandSettings(this->m_payload.body.active_bands)
    , m_receiver(this->m_payload.body.receiver)
    , m_bands(other.m_bands)
    , m_modes(other.m_modes)
    , m_cache(other.m_cache)
  {
  }

  RadioSettingsBaseT(const PayloadPbType& payload, bool complete = true)
    : MessageBase(payload, complete)
    , m_deemComplete(false)
    , m_traverser(&MessageBase::body(), descriptor)
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

  void deemComplete(bool deemComplete) { m_deemComplete = deemComplete; }
  [[nodiscard]] bool deemComplete() const { return m_deemComplete; }

  ResultCode setAllFieldsPresence(bool present)
  {
    return m_traverser.setAllFieldsPresence(present);
  }

  ResultCode updateField(const FieldPath &path, const FieldUpdateVariant &value)
  {
    return m_traverser.updateField(path, value);
  }

  ResultCode getField(const FieldPath &path, FieldUpdateVariant &value) const
  {
    return m_traverser.getField(path, value);
  }

  ResultCode getField(
    const FieldPath &path,
    FieldUpdateVariant &value,
    bool mustHave,
    bool parentsMustHave,
    bool& retrieved
  )
  {
    return m_traverser.getField(path, value, mustHave, parentsMustHave, retrieved);
  }

  ResultCode setFieldPresence(const FieldPath &path, bool present)
  {
    return m_traverser.setFieldPresence(path, present);
  }

  ResultCode mergePresentFields(const void* pRhsMessage)
  {
    return m_traverser.mergePresentFields(pRhsMessage);
  }

  ResultCode replace(IMessage& other, bool deemComplete)
  {
    if (other.getPayloadType() != static_cast<PayloadType>(payloadTypeEnum)) {
      return ResultCode::ERR_PROTOBUF_PAYLOAD_MISMATCH;
    }
    const auto& otherAsSameType = static_cast<const RadioSettingsBaseT&>(other);
    return replace(otherAsSameType.body(), deemComplete);
  }

  ResultCode merge(IMessage& other)
  {
    if (other.getPayloadType() != static_cast<PayloadType>(payloadTypeEnum)) {
      return ResultCode::ERR_PROTOBUF_PAYLOAD_MISMATCH;
    }
    const auto& otherAsSameType = static_cast<const RadioSettingsBaseT&>(other);
    return merge(otherAsSameType.body());
  }

  virtual ResultCode replace(const SettingsPbType& update, bool deemComplete)
  {
    MessageBase::m_payload.body = update;
    m_deemComplete = deemComplete;
    if (deemComplete) {
      setAllFieldsPresence(true);
    }
    return ResultCode::OK;
  };

  virtual ResultCode merge(const SettingsPbType& update)
  {
    return m_traverser.mergePresentFields(&update);
  }

  ResultCode applyFieldUpdate(const FieldUpdate &settingUpdate) override
  {
    ResultCode rc = ResultCode::OK;
    if (settingUpdate.isIndirect()) {
      rc = updateIndirectField(settingUpdate);
    } else {
      rc = m_traverser.updateField(settingUpdate);
    }
    if (rc == ResultCode::OK && settingUpdate.needsAutoComplete()) {
      rc = autoComplete(settingUpdate.descriptor());
    }
    return rc;
  }

  ResultCode updateIndirectField(const FieldUpdate &settingUpdate)
  {
    if (settingUpdate.descriptor().getPath().at(0) == COMMON_ACTIVE_BANDS_TAG) {
      ResultCode rc = m_activeBandSettings.updateIndirectField(settingUpdate, 1);
      if (rc == ResultCode::OK) {
        this->m_payload.body.has_active_bands = true;
      }
      return rc;
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
  bool m_deemComplete;
  MessageTraverser m_traverser;

  ActiveBandSettingsClass m_activeBandSettings;
  ReceiverSettings m_receiver;

  const BandCategoryList* m_bands;
  const ModeList* m_modes;
  CacheClass*  m_cache;
};
