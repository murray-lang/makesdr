#pragma once
#include <atomic>
#include <transport/freertos/client/IqCapableClientTransportT.h>
#include <transport/stm32h745i/globalStm32H745SingleIqRxRings.h>
#include <settings/model/radio/RadioSettingsMirrorT.h>
#include <radios/base/RadioBase.h>
#include <thread/Semaphore.h>

#include <stm32h745i_discovery.h>

using Transport = IqCapableClientTransportT<
    Stm32H745SingleIqRxSettingsRings,
    Stm32H745UpdateMessageRings,
    Stm32H745MetaMessageRings,
    Stm32H745RxMeteringMessageRings,
    Stm32H745SpectrumMessageRings
  >;

class RadioClient :
  public IRadioSettingsUpdater,
  public RadioBase,
  public MessageSinkT<CapabilitiesMessage>,
  public MessageSinkT<BandCategoryList>,
  public MessageSinkT<ModeList>,
  public MessageSinkT<RxMeteringMessage>,
  public MessageSinkT<SpectrumMessage>
{
public:

  using RadioSettings = Transport::RadioSettings;

  using SettingsMirror = RadioSettingsMirrorT<Transport, RadioClient>;

  using MessageSinkT<BandCategoryList>::applyMessage;
  using MessageSinkT<ModeList>::applyMessage;
  using MessageSinkT<RxMeteringMessage>::applyMessage;
  using MessageSinkT<SpectrumMessage>::applyMessage;

  RadioClient();

  ResultCode configure(const Config::Radio::Fields& config)  final;

  ResultCode start() override;
  void stop() override;

  ResultCode requestAll();

  ResultCode requestSettings()
  {
    return m_settingsMirror.requestSettings();
  }

  ResultCode requestCapabilities() { return m_requester.requestCapabilities(); }
  ResultCode requestCurrentSettings() { return m_requester.requestSettings(); }
  ResultCode requestModes() { return m_requester.requestModes(); }
  ResultCode requestBands() { return m_requester.requestBands(); }

  ResultCode applyFieldUpdate(const FieldUpdate& update) final;


  // ResultCode applyMessage(RadioSettings* message) final;
  ResultCode applyMessage(CapabilitiesMessage* message) final;
  ResultCode applyMessage(BandCategoryList* message) final;
  ResultCode applyMessage(ModeList* message) final;
  ResultCode applyMessage(RxMeteringMessage* message) final;
  ResultCode applyMessage(SpectrumMessage* message) final;

  template<typename S>
  ResultCode applySettings(const S& settings, const S& changes)
  {

    notifyReceived(Awaiting::Settings);
    return ResultCode::OK;
  }

  RadioSettingsUpdater* getUpdater() { return m_pUpdater; }

  // IRadioSettingsUpdater overrides
  ResultCode ptt(bool on) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->ptt(on) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode selectBand(const char* bandName, bool final) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->selectBand(bandName, final) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setMultiPipeline(SplitBandId bandId, bool isMulti, bool final) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setMultiPipeline(bandId, isMulti, final) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode closePipeline(SplitBandId bandId, PipelineId pipelineId, bool final) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->closePipeline(bandId, pipelineId, final) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setTxBand(SplitBandId bandId, bool final) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setTxBand(bandId, final) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setTxPipeline(SplitBandId bandId, PipelineId pipelineId, bool final) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setTxPipeline(bandId, pipelineId, final) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setFocusBand(SplitBandId bandId, bool final) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setFocusBand(bandId, final) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setFocusPipeline(SplitBandId bandId, PipelineId pipelineId, bool final) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setFocusPipeline(bandId, pipelineId, final) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode mutePipeline(SplitBandId bandId, PipelineId pipelineId, bool mute, bool final) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->mutePipeline(bandId, pipelineId, mute, final) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode split(bool final) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->split(final) : ResultCode::ERR_UPDATER_NOT_SET;

  }
  ResultCode unsplit(SplitBandId closeBandId, bool final) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->unsplit(closeBandId, final) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setFocusMode(Mode::Type modeType, bool final) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setFocusMode(modeType, final) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setFocusAgc(AgcSpeed agcSpeed, bool final) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setFocusAgc(agcSpeed, final) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setCentreFrequency(int64_t frequency, bool final) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setCentreFrequency(frequency, final) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode stepCentreFrequency(int32_t steps, bool final) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->stepCentreFrequency(steps, final) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setFocusPipelineFrequency(int64_t frequency, bool final) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setFocusPipelineFrequency(frequency, final) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode stepFocusPipelineFrequency(int32_t steps, bool final) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->stepFocusPipelineFrequency(steps, final) : ResultCode::ERR_UPDATER_NOT_SET;
  }

private:
  enum class Awaiting : uint8_t { None, Capabilities, Modes, Bands, Settings };

  ResultCode requestAndWait(Awaiting what, ResultCode (RadioClient::*requestFunc)(), uint32_t timeoutMs);
  // Called from applyMessage()/applySettings() on the transport task
  void notifyReceived(Awaiting what);

private:
  Transport m_transport;
  SettingsMirror m_settingsMirror;
  RadioSettingsUpdater* m_pUpdater;
  BandCategoryList m_bands;
  ModeList m_modes;
  RadioRequester m_requester;

  std::atomic<Awaiting> m_awaiting{Awaiting::None};
  Semaphore m_replied;
};
