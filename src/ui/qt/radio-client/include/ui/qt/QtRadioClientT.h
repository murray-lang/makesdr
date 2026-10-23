#pragma once
#include <type_traits>

#include <QEventLoop>
#include <QTimer>
// #include <ui/qt/QtRadioClientBase.h>
#include <config/struct/RadioConfig.h>
#include <samples/SampleTypes.h>
#include <transport/SettingsClientSideRingsT.h>
#include <transport/MetaClientSideRingsT.h>
#include <transport/IqClientSideRingsT.h>
#include <transport/qt/QtRecipientT.h>
// #include <transport/qt/event/globalQtMessageExchange.h>
#include <settings/model/radio/RadioSettingsUpdater.h>
#include <settings/model/radio/RadioRequester.h>
#include <radios/base/RadioBase.h>
#include <transport/qt/globalQtDualIqSplitRings.h>
#include <transport/qt/globalQtDualIqRxTxRings.h>
#include <transport/qt/globalQtDualIqRxRings.h>
// #include <transport/qt/globalQtSingleIqRxTxRings.h>
#include <transport/qt/globalQtSingleIqRxRings.h>

#include <transport/qt/globalQtMetaRings.h>
#include <transport/qt/globalQtIqRings.h>
#include <transport/qt/globalQtUpdateRings.h>

#include <transport/qt/QtMetaClientSideTransport.h>

#include <transport/qt/QtIqClientSideTransport.h>
#include <transport/qt/QtRxMeteringClientSideTransport.h>
#include <transport/qt/QtSettingsClientSideTransportT.h>
#include <transport/qt/QtSpectrumClientSideTransport.h>
#include <transport/qt/QtUpdateClientSideTransport.h>

#include <transport/qt/globalQtRxMeteringRings.h>
#include <transport/qt/globalQtSpectrumRings.h>

#include <settings/model/radio/IRadioSettingsMirror.h>
#include <settings/model/radio/RadioSettingsMirrorPolymorphicT.h>
#include <settings/model/radio/RadioSettingsMirrorT.h>
#include <settings/model/radio/iq/DualIqRxSettings.h>
#include <settings/model/radio/iq/DualIqRxTxSettings.h>
#include <settings/model/radio/iq/SingleIqRxSettings.h>
#include <settings/model/radio/iq/SingleIqRxTxSettings.h>


class SingleIqRxSettings;
class SingleIqRxTxSettings;

class QtRadioClientT :
  public IRadioSettingsUpdater,
  public RadioBase,
  // public MessageSinkT<RadioSettingsT>,
  public MessageSinkT<CapabilitiesMessage>,
  public MessageSinkT<BandCategoryList>,
  public MessageSinkT<ModeList>,
  public MessageSinkT<RxMeteringMessage>,
  public MessageSinkT<SpectrumMessage>
{
public:
  // using RadioSettings = RadioSettingsT;
  using MessageSinkT<BandCategoryList>::applyMessage;
  using MessageSinkT<ModeList>::applyMessage;
  using MessageSinkT<RxMeteringMessage>::applyMessage;
  using MessageSinkT<SpectrumMessage>::applyMessage;

  QtRadioClientT(QObject* parent)
  : m_pUpdater(nullptr)
  , m_metaTransport(globalQtMetaRings())
  , m_rxMeteringTransport(globalQtRxMeteringRings())
  // , m_iqTransport(globalQtIqRings())
  , m_spectrumTransport(globalQtSpectrumRings())
  , m_updateTransport(globalQtUpdateRings())
  {
    // m_settingsTransport.connectRadioSettingsSink(this);
    m_metaTransport.connectCapabilitiesSink(this);
    m_metaTransport.connectBandsSink(this);
    m_metaTransport.connectModesSink(this);
    m_rxMeteringTransport.connectRxMeteringSink(this);
    // m_iqTransport.connectIqSink(this);
    m_spectrumTransport.connectSpectrumSink(this);

    m_requester.connectFieldUpdateSink(&m_updateTransport);
  }

  // Detach before m_recipient is destroyed, so nothing more is posted to it.
  ~QtRadioClientT() override
  {
    QtRadioClientT::stop();
  }

  // nullptr until capabilities have been received and the settings mirror created
  [[nodiscard]] const IRadioSettingsView* getSettings() const
  {
    return m_settingsMirror != nullptr ? m_settingsMirror->view() : nullptr;
  }
  [[nodiscard]] const BandCategoryList* getBands() const { return &m_bands; }
  [[nodiscard]] const ModeList* getModes() const { return &m_modes; }
  RadioSettingsUpdater* getUpdater() { return m_pUpdater; }

  // Nothing to configure: the channels are fixed.
  ResultCode configure(const Config::Radio::Fields& config) override
  {
    return ResultCode::OK;
  }

  ResultCode start() override
  {
    // m_settingsTransport.attachRecipient();
    m_metaTransport.attachRecipient();
    m_rxMeteringTransport.attachRecipient();
    // m_iqTransport.attachRecipient();
    m_spectrumTransport.attachRecipient();
    if (m_settingsMirror != nullptr) m_settingsMirror->start();
    return ResultCode::OK;
  }

  void stop() override
  {
    // m_settingsTransport.detachRecipient();
    m_metaTransport.detachRecipient();
    m_rxMeteringTransport.detachRecipient();
    // m_iqTransport.detachRecipient();
    m_spectrumTransport.detachRecipient();
    if (m_settingsMirror != nullptr) m_settingsMirror->stop();
  }

  ResultCode requestAll()
  {
    ResultCode rc = requestAndWait(
      m_waitingForCapabilities,
      &QtRadioClientT::requestCapabilities,
      1000
    );
    if (rc != ResultCode::OK) {
      qDebug() << "Failed to request capabilities";
      return rc;
    }

    rc = requestAndWait(
      m_waitingForModes,
      &QtRadioClientT::requestModes,
      1000
    );
    if (rc != ResultCode::OK) {
      qDebug() << "Failed to request modes";
      return rc;
    }

    rc = requestAndWait(
      m_waitingForBands,
      &QtRadioClientT::requestBands,
      1000
    );
    if (rc != ResultCode::OK) {
      qDebug() << "Failed to request bands";
      return rc;
    }

    // rc = requestAndWait(m_waitingForSettings, &QtRadioClientT::requestCurrentSettings, 1000);
    // if (rc != ResultCode::OK) {
    //   qDebug() << "Failed to request settings";
    //   return rc;
    // }

    return ResultCode::OK;
  }

  ResultCode applyMessage(CapabilitiesMessage* message) final
  {
    if (m_waitingForCapabilities && m_pWaitLoop != nullptr) {
      m_waitingForCapabilities = false;
      m_pWaitLoop->quit();
    }
    if (message == nullptr) return ResultCode::OK;
    if (m_settingsMirror != nullptr) {
      m_settingsMirror->stop();
      m_pUpdater = nullptr;
    }
    switch (message->capabilities().getSettingsType()) {
    case PayloadType::PAYLOAD_TYPE_SETTINGS_SPLIT_BAND_DUAL_IQ_RXTX:
      m_settingsMirror = makeSettingsMirror<SplitBandDualIqRxTxSettings>(globalQtDualIqSplitRings());
      break;
    case PayloadType::PAYLOAD_TYPE_SETTINGS_DUAL_IQ_RXTX:
      m_settingsMirror = makeSettingsMirror<DualIqRxTxSettings>(globalQtDualIqRxTxRings());
      break;

    case PayloadType::PAYLOAD_TYPE_SETTINGS_DUAL_IQ_RX:
      m_settingsMirror = makeSettingsMirror<DualIqRxSettings>(globalQtDualIqRxRings());
      break;
    // case PayloadType::PAYLOAD_TYPE_SETTINGS_SINGLE_IQ_RXTX: // TODO: globalQtSingleIqRxTxRings not defined yet!
    //   m_settingsMirror = makeSettingsMirror<SingleIqRxTxSettings>(globalQtSingleIqRxTxRings());
    //   break;
    case PayloadType::PAYLOAD_TYPE_SETTINGS_SINGLE_IQ_RX:
      m_settingsMirror = makeSettingsMirror<SingleIqRxSettings>(globalQtSingleIqRxRings());
      break;
    default:
      return ResultCode::ERR_PAYLOAD_NOT_SETTING_RELATED;
    }
    m_pUpdater = m_settingsMirror->updater();
    m_settingsMirror->start();
    ResultCode rc = requestAndWait(
      m_waitingForSettings,
      &QtRadioClientT::requestSettings,
      2000
    );
    return rc;
  }

  ResultCode requestSettings()
  {
    return m_settingsMirror->requestSettings();
  }

  template<typename S>
  std::unique_ptr<IRadioSettingsMirror> makeSettingsMirror(
    QtSettingsMessageRingsT<S>& settingsRings
    )
  {
    using Mirror = RadioSettingsMirrorT<S, QtSettingsClientSideTransportT<S>, QtUpdateClientSideTransport, QtRadioClientT>;
    using PolymorphicMirror = RadioSettingsMirrorPolymorphicT<Mirror>;

    return std::make_unique<PolymorphicMirror>(
      settingsRings,
      m_updateTransport,
      this
      );
  }

  // Called by the settings mirror with its concrete type. The UI only sees views:
  // getSettings() for the accumulated state, and the emitted view for what changed.
  // The state view (and the band views it hands out) persist with the mirror; the changes
  // view only lives for the duration of the (direct) signal emission.
  template<typename S>
  ResultCode applySettings(const S& /*state*/, const S& changes)
  {
    if (m_waitingForSettings && m_pWaitLoop != nullptr) {
      m_waitingForSettings = false;
      m_pWaitLoop->quit();
    }
    const RadioSettingsViewT<S> changesView(changes, m_settingsMirror->view());
    emitRadioSettingsReceived(&changesView);
    return ResultCode::OK;
  }

  // ResultCode applyMessage(RadioSettingsT* message) final
  // {
  //   if (m_waitingForSettings && m_pWaitLoop != nullptr) {
  //     m_waitingForSettings = false;
  //     m_pWaitLoop->quit();
  //   }
  //   if (message != nullptr) {
  //     ResultCode rc = applySettings(*message);
  //     if (rc == ResultCode::OK) {
  //       emitRadioSettingsReceived(message);
  //     }
  //     return rc;
  //   }
  //   return ResultCode::OK;
  // }

  ResultCode applyMessage(BandCategoryList* message) final
  {
    setBands(message);
    if (m_waitingForBands && m_pWaitLoop != nullptr) {
      m_waitingForBands = false;
      m_pWaitLoop->quit();
    }
    return ResultCode::OK;
  }

  ResultCode applyMessage(ModeList* message) final
  {
    setModes(message);
    if (m_waitingForModes && m_pWaitLoop != nullptr) {
      m_waitingForModes = false;
      m_pWaitLoop->quit();
    }
    return ResultCode::OK;
  }

  ResultCode applyMessage(RxMeteringMessage* message) final
  {
    emitRxMeteringReceived(message);
    return ResultCode::OK;
  }

  ResultCode applyMessage(SpectrumMessage* message) final
  {
    emitSpectrumReceived(message);
    return ResultCode::OK;
  }

  // ResultCode applySettings(RadioSettingsT& settings) override
  // {
  //   // emit radioSettingsReceived(settings, settings->sequence);
  //   return ResultCode::OK;
  // }

  ResultCode applyFieldUpdate(const FieldUpdate& update) override
  {
    return m_updateTransport.applyFieldUpdate(update);
  }

  virtual void setBands(BandCategoryList* message) // virtual for mocking only
  {
    if (message != nullptr) {
      m_bands = *message;
    }
  }

  virtual void setModes(ModeList* message)  // virtual for mocking only
  {
    if (message != nullptr) {
      m_modes = *message;
    }
  }
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

protected:
  virtual void emitCapabilitiesReceived(const Capabilities* caps) = 0;
  virtual void emitRadioSettingsReceived(const IRadioSettingsView* settings) = 0;

  virtual void emitRxMeteringReceived(const RxMeteringMessage* iq) = 0;
  virtual void emitSpectrumReceived(const SpectrumMessage* spectrum) = 0;

  ResultCode requestCapabilities() { return m_requester.requestCapabilities(); }
  ResultCode requestCurrentSettings() { return m_requester.requestSettings(); }
  ResultCode requestModes() { return m_requester.requestModes(); }
  ResultCode requestBands() { return m_requester.requestBands(); }

  ResultCode requestAndWait(
        bool& waitingFlag,
        ResultCode (QtRadioClientT::*requestFunc)(),
        int timeoutMs)
  {
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);

    // Set up wait state
    waitingFlag = true;
    m_pWaitLoop = &loop;

    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

    // Call the request function
    ResultCode rc = (this->*requestFunc)();
    if (rc != ResultCode::OK) {
      waitingFlag = false;
      m_pWaitLoop = nullptr;
      return rc;
    }

    // Wait for response
    timer.start(timeoutMs);
    loop.exec();

    // Clean up
    bool received = !waitingFlag; // If still waiting, we timed out
    waitingFlag = false;
    m_pWaitLoop = nullptr;

    return received ? ResultCode::OK : ResultCode::ERR_RADIO_REQUEST_TIMEOUT;
  }

private:
  std::unique_ptr<IRadioSettingsMirror> m_settingsMirror;
  BandCategoryList m_bands;
  ModeList m_modes;
  RadioRequester m_requester;
  RadioSettingsUpdater* m_pUpdater;

  QtMetaClientSideTransport m_metaTransport;
  QtRxMeteringClientSideTransport m_rxMeteringTransport;
  // QtIqClientSideTransport m_iqTransport;
  QtSpectrumClientSideTransport m_spectrumTransport;
  QtUpdateClientSideTransport m_updateTransport;
  bool m_waitingForCapabilities = false;
  bool m_waitingForModes = false;
  bool m_waitingForBands = false;
  bool m_waitingForSettings = false;
  QEventLoop* m_pWaitLoop = nullptr;
};
