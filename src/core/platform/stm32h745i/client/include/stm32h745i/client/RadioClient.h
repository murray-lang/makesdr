#pragma once
#include <transport/freertos/client/IqCapableClientTransportT.h>
#include <transport/stm32h745i/globalStm32H745SingleIqRxRings.h>
#include <settings/model/radio/RadioSettingsMirrorT.h>
#include <radios/base/RadioBase.h>

#include <stm32h745i_discovery.h>

using Transport = IqCapableClientTransportT<
    Stm32H745SingleIqRxSettingsRings,
    Stm32H745UpdateMessageRings,
    Stm32H745MetaMessageRings,
    Stm32H745RxMeteringMessageRings,
    Stm32H745SpectrumMessageRings
  >;

using SettingsMirror = RadioSettingsMirrorT<Transport>;
using ISettingsListener = SettingsMirror::IListener;

//*****************************************************************************
// The CM7's client of the radio on the CM4.
//
// Single-threaded: the task that calls start() (e.g. the LVGL task) hosts the
// transport, so incoming messages, the settings mirror and the listeners all
// run on it, with no locking. That task calls waitAndDispatch() in its loop in
// place of its delay, and must not block waiting for a reply, so requestAll()
// is a sequence advanced by the replies themselves.
//
// Spectrum and metering go straight to the sinks connected for them. Settings
// go through the mirror, whose listener receives the accumulated state and,
// separately, what changed.
//*****************************************************************************
class RadioClient :
  public ISettingsListener,
  public IRadioSettingsUpdater,
  public RadioBase,
  public MessageSinkT<CapabilitiesMessage>,
  public MessageSinkT<BandCategoryList>,
  public MessageSinkT<ModeList>
{
public:

  using RadioSettings = Transport::RadioSettings;

  using MessageSinkT<CapabilitiesMessage>::applyMessage;
  using MessageSinkT<BandCategoryList>::applyMessage;
  using MessageSinkT<ModeList>::applyMessage;

  RadioClient();

  ResultCode configure(const Config::Radio::Fields& config)  final;

  // Makes the calling task the one that hosts the transport.
  ResultCode start() override;
  void stop() override;

  // The hosting task's wait: delivers incoming messages and times out the
  // requestAll() sequence. Returns true if anything was waiting.
  bool waitAndDispatch(TickType_t timeout);

  // Starts requesting modes, bands then settings, each once the previous has
  // been answered. Returns at once; see requestAllStatus().
  ResultCode requestAll();

  // ERR_RADIO_REQUEST_TIMEOUT (or a send error) if the sequence failed,
  // OK otherwise, including while it is still in progress.
  [[nodiscard]] ResultCode requestAllStatus() const { return m_requestAllStatus; }
  [[nodiscard]] bool requestAllDone() const { return m_awaiting == Awaiting::None; }

  // Connect before start().
  void connectSettingsListener(ISettingsListener* listener) { m_pSettingsListener = listener; }
  void connectSpectrumSink(MessageSinkT<SpectrumMessage>* sink) { m_transport.connectSpectrumSink(sink); }
  void connectRxMeteringSink(MessageSinkT<RxMeteringMessage>* sink) { m_transport.connectRxMeteringSink(sink); }

  [[nodiscard]] const RadioSettings& settings() const { return m_settingsMirror.settings(); }
  [[nodiscard]] const BandCategoryList& bands() const { return m_bands; }
  [[nodiscard]] const ModeList& modes() const { return m_modes; }

  ResultCode requestSettings()
  {
    return m_settingsMirror.requestSettings();
  }

  ResultCode requestCapabilities() { return m_requester.requestCapabilities(); }
  ResultCode requestCurrentSettings() { return m_requester.requestSettings(); }
  ResultCode requestModes() { return m_requester.requestModes(); }
  ResultCode requestBands() { return m_requester.requestBands(); }

  ResultCode applyFieldUpdate(const FieldUpdate& update) final;


  ResultCode applyMessage(CapabilitiesMessage* message) final;
  ResultCode applyMessage(BandCategoryList* message) final;
  ResultCode applyMessage(ModeList* message) final;

  // Called by the settings mirror, on the hosting task.
  // template<typename S>
  // ResultCode applySettings(const S& settings, const S& changes)
  // {
  //   if (m_pSettingsListener != nullptr) {
  //     m_pSettingsListener->radioSettingsChanged(settings, changes);
  //   }
  //   notifyReceived(Awaiting::Settings);
  //   return ResultCode::OK;
  // }
  void radioSettingsChanged(const RadioSettings& state, const RadioSettings& changes) override
  {
    if (m_pSettingsListener != nullptr) {
      m_pSettingsListener->radioSettingsChanged(state, changes);
    }
    notifyReceived(Awaiting::Settings);
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

  static constexpr uint32_t RequestTimeoutMs = 2000;

  // Sends the request for 'what' and starts its timeout.
  ResultCode request(Awaiting what);
  void failRequestAll(ResultCode rc);
  // Called from applyMessage()/applySettings(): sends the next request.
  void notifyReceived(Awaiting what);

private:
  Transport m_transport;
  SettingsMirror m_settingsMirror;
  RadioSettingsUpdater* m_pUpdater;
  BandCategoryList m_bands;
  ModeList m_modes;
  RadioRequester m_requester;

  ISettingsListener* m_pSettingsListener;

  // The hosting task only
  Awaiting   m_awaiting{Awaiting::None};
  TickType_t m_requestSentAt{0};
  ResultCode m_requestAllStatus{ResultCode::OK};
};
