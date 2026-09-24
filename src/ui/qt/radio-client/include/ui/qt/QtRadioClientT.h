#pragma once
#include <type_traits>

#include <QEventLoop>
#include <QTimer>
// #include <ui/qt/QtRadioClientBase.h>
#include <config/struct/RadioConfig.h>
#include <samples/SampleTypes.h>
#include <transport/radio/ClientTransportT.h>
#include <transport/qt/event/QtWakeT.h>
#include <transport/qt/event/globalQtMessageExchange.h>
#include <settings/model/data/radio/RadioLookup.h>
#include <settings/model/radios/RadioSettingsUpdater.h>
#include <settings/model/radios/RadioSettingsRequester.h>
#include <radios/base/RadioBaseT.h>



template <typename RadioSettingsT>
class QtRadioClientT :
  public IRadioSettingsUpdater,
  public RadioBaseT<RadioSettingsT>,
  public MessageSinkT<RadioSettingsT>,
  public MessageSinkT<IqMessage>,
  public MessageSinkT<BandCategoryList>,
  public MessageSinkT<ModeList>
{
  static_assert(std::is_same_v<RadioSettingsT, QtRadioMessageExchange::Settings>,
                "The client's settings type must match the build's RadioSettings");

public:
  using CacheType = typename RadioSettingsT::Cache;
  using MessageSinkT<RadioSettingsT>::applyMessage;
  using MessageSinkT<BandCategoryList>::applyMessage;
  using MessageSinkT<ModeList>::applyMessage;

  QtRadioClientT(QObject* parent)
  :  m_settings{}
  // , m_rawLookupStruct{}
  , m_bands{}
  , m_modes{}
  , m_pUpdater(m_settings.updater())
  , m_transport(globalQtMessageExchange())
  , m_wake(m_transport)   // stays on the constructing (GUI) thread
  , m_waitingForSettings(false)
  , m_waitingForModes(false)
  , m_waitingForBands(false)
  , m_pWaitLoop(nullptr)
  {
    m_transport.connectRadioSettingsSink(this);
    m_transport.connectIqSink(this);
    m_transport.connectBandsSink(this);
    m_transport.connectModesSink(this);

    m_requester.connectFieldUpdateSink(&m_transport);
    if (m_pUpdater != nullptr) {
      m_pUpdater->connectFieldUpdateSink(&m_transport);
    }
  }

  // Detach before m_wake is destroyed, so nothing more is posted to it.
  ~QtRadioClientT() override
  {
    m_transport.detach();
  }

  [[nodiscard]] const RadioSettingsT* getSettings() const override { return &m_settings; }
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
    m_transport.attach(&m_wake);
    return ResultCode::OK;
  }

  void stop() override
  {
    m_transport.detach();
  }

  ResultCode requestAll()
  {
    ResultCode rc = requestAndWait<ModeList>(m_waitingForModes, &QtRadioClientT::requestModes, 1000);
    if (rc != ResultCode::OK) return rc;

    rc = requestAndWait<BandCategoryList>(m_waitingForBands, &QtRadioClientT::requestBands, 1000);
    if (rc != ResultCode::OK) return rc;

    rc = requestAndWait<RadioSettingsT>(m_waitingForSettings, &QtRadioClientT::requestCurrentSettings, 1000);
    if (rc != ResultCode::OK) return rc;

    return ResultCode::OK;
  }

  ResultCode applyMessage(RadioSettingsT* message) final
  {
    if (message != nullptr) {
      ResultCode rc = ResultCode::OK;
      if (message->purpose() == PayloadPurpose::PURPOSE_REPLACE) {
        m_settings.setPurpose(PayloadPurpose::PURPOSE_REPLACE);
        rc = m_settings.replace(*message, true);
      } else {
        rc = m_settings.merge(*message);
      }
      if (rc == ResultCode::OK) {
        rc = applySettings(m_settings);
      }
      if (m_waitingForSettings && m_pWaitLoop != nullptr) {
        m_waitingForSettings = false;
        m_pWaitLoop->quit();
      }
      if (rc == ResultCode::OK) {
        emitRadioSettingsReceived(&m_settings);
      }
      return rc;
    }
    return ResultCode::OK;
  }

  ResultCode applyMessage(IqMessage* message) final
  {
    emitReceiverIqReceived(message);
    return ResultCode::OK;
  }

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

  ResultCode applySettings(RadioSettingsT& settings) override
  {
    // emit radioSettingsReceived(settings, settings->sequence);
    return ResultCode::OK;
  }

  ResultCode applyFieldUpdate(const FieldUpdate& update) override
  {
    return m_transport.applyFieldUpdate(update);
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
  ResultCode selectBand(const char* bandName) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->selectBand(bandName) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setMultiPipeline(SplitBandId bandId, bool isMulti) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setMultiPipeline(bandId, isMulti) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode closePipeline(SplitBandId bandId, PipelineId pipelineId) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->closePipeline(bandId, pipelineId) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setTxBand(SplitBandId bandId) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setTxBand(bandId) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setTxPipeline(SplitBandId bandId, PipelineId pipelineId) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setTxPipeline(bandId, pipelineId) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setFocusBand(SplitBandId bandId) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setFocusBand(bandId) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setFocusPipeline(SplitBandId bandId, PipelineId pipelineId) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setFocusPipeline(bandId, pipelineId) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode mutePipeline(SplitBandId bandId, PipelineId pipelineId, bool mute) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->mutePipeline(bandId, pipelineId, mute) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode split() override
  {
    return m_pUpdater != nullptr ? m_pUpdater->split() : ResultCode::ERR_UPDATER_NOT_SET;

  }
  ResultCode unsplit(SplitBandId closeBandId) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->unsplit(closeBandId) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setFocusMode(Mode::Type modeType) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setFocusMode(modeType) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setCentreFrequency(int64_t frequency) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setCentreFrequency(frequency) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode stepCentreFrequency(int32_t steps) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->stepCentreFrequency(steps) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode setFocusPipelineFrequency(int64_t frequency) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->setFocusPipelineFrequency(frequency) : ResultCode::ERR_UPDATER_NOT_SET;
  }
  ResultCode stepFocusPipelineFrequency(int32_t steps) override
  {
    return m_pUpdater != nullptr ? m_pUpdater->stepFocusPipelineFrequency(steps) : ResultCode::ERR_UPDATER_NOT_SET;
  }

protected:
  virtual void emitRadioSettingsReceived(const RadioSettingsT* settings) = 0;
  virtual void emitReceiverIqReceived(const IqMessage* iq) = 0;
  ResultCode requestCurrentSettings() { return m_requester.requestSettings(); }
  ResultCode requestModes() { return m_requester.requestModes(); }
  ResultCode requestBands() { return m_requester.requestBands(); }

  template<typename TMessage>
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
  RadioSettingsT m_settings;
  // RadioLookup::Proto m_rawLookupStruct;
  BandCategoryList m_bands;
  ModeList m_modes;
  RadioSettingsRequester m_requester;
  RadioSettingsUpdater* m_pUpdater;

  using Transport = ClientTransportT<QtRadioMessageExchange>;
  // Declaration order matters: m_wake refers to m_transport, so is destroyed first.
  Transport m_transport;
  QtWakeT<Transport> m_wake;
  bool m_waitingForSettings = false;
  bool m_waitingForModes = false;
  bool m_waitingForBands = false;
  QEventLoop* m_pWaitLoop = nullptr;
};