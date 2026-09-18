#pragma once
#include <QThread>
#include <QTimer>
// #include <ui/qt/QtRadioClientBase.h>
#include <config/struct/RadioConfig.h>
#include <samples/SampleTypes.h>
#include <transport/in/QtTransportInT.h>
#include <transport/out/QtTransportOutT.h>
#include <transport/out/QtTransportFieldUpdateOut.h>
#include <settings/model/data/radio/RadioLookup.h>
#include <settings/model/radios/RadioSettingsUpdater.h>
#include <settings/model/radios/RadioSettingsRequester.h>
#include <radios/base/RadioBaseT.h>



template <typename RadioSettingsT>
class QtRadioClientT :
  public IRadioSettingsUpdater,
  public RadioBaseT<RadioSettingsT>,
  public MessageSinkT<RadioSettingsT>,
  public MessageSinkT<BandCategoryList>,
  public MessageSinkT<ModeList>
{
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
  , m_settingsIn(Config::QtTransport::Target::Client, Config::QtTransport::Message::Settings)
  // , m_cacheIn(Config::QtTransport::Role::Client, Config::QtTransport::Target::Setting)
  , m_bandsIn(Config::QtTransport::Target::Client, Config::QtTransport::Message::Bands)
  , m_modesIn(Config::QtTransport::Target::Client, Config::QtTransport::Message::Modes)
  , m_settingsOut(Config::QtTransport::Target::Radio, Config::QtTransport::Message::Settings)
  , m_fieldUpdateOut(Config::QtTransport::Target::Radio)
  , m_waitingForSettings(false)
  , m_waitingForModes(false)
  , m_waitingForBands(false)
  , m_pWaitLoop(nullptr)
  {
    // Move all handlers to the same thread
    m_settingsIn.moveToThread(QThread::currentThread());
    // m_cacheIn.moveToThread(&m_transportThread);
    m_bandsIn.moveToThread(QThread::currentThread());
    m_modesIn.moveToThread(QThread::currentThread());

    m_settingsIn.connectMessageSink(this);
    m_bandsIn.connectMessageSink(this);
    m_modesIn.connectMessageSink(this);
    // m_lookupIn.connectMessageSink(this);

    m_requester.connectFieldUpdateSink(&m_fieldUpdateOut);
    if (m_pUpdater != nullptr) {
      m_pUpdater->connectFieldUpdateSink(&m_fieldUpdateOut);
    }
  }

  ~QtRadioClientT() override
  {
    // m_transportThread.quit();
    // m_transportThread.wait();
  }

  [[nodiscard]] const RadioSettingsT* getSettings() const override { return &m_settings; }
  [[nodiscard]] const BandCategoryList* getBands() const { return &m_bands; }
  [[nodiscard]] const ModeList* getModes() const { return &m_modes; }
  RadioSettingsUpdater* getUpdater() { return m_pUpdater; }

  ResultCode configure(const Config::Radio::Fields& config) override
  {
    ResultCode rc = m_settingsIn.configure();
    if (rc != ResultCode::OK) return rc;
    rc = m_bandsIn.configure();
    if (rc != ResultCode::OK) return rc;
    rc = m_modesIn.configure();
    if (rc != ResultCode::OK) return rc;

    rc = m_settingsOut.configure();
    if (rc != ResultCode::OK) return rc;
    rc = m_fieldUpdateOut.configure();
    if (rc != ResultCode::OK) return rc;

    rc = m_settingsIn.setQtEventTarget();
    if (rc != ResultCode::OK) return rc;
    rc = m_bandsIn.setQtEventTarget();
    if (rc != ResultCode::OK) return rc;
    rc = m_modesIn.setQtEventTarget();
    if (rc != ResultCode::OK) return rc;

    return rc;
  }

  ResultCode start() override
  {
    ResultCode rc = m_settingsIn.open();
    if (rc != ResultCode::OK) return rc;
    rc = m_bandsIn.open();
    if (rc != ResultCode::OK) return rc;
    rc = m_modesIn.open();
    if (rc != ResultCode::OK) return rc;
    rc = m_settingsOut.open();
    if (rc != ResultCode::OK) return rc;
    rc = m_fieldUpdateOut.open();
    if (rc != ResultCode::OK) return rc;

    return ResultCode::OK;
  }

  void stop() override
  {
    // m_transportThread.quit();
    // m_transportThread.wait();
    m_settingsIn.close();
    m_bandsIn.close();
    m_modesIn.close();
    m_settingsOut.close();
    m_fieldUpdateOut.close();
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
    return m_fieldUpdateOut.applyFieldUpdate(update);
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
  // QThread m_transportThread;  // Single thread for all incoming messages
  QtTransportInT<RadioSettingsT> m_settingsIn;
  // QtTransportInT<CacheType> m_cacheIn;
  QtTransportInT<BandCategoryList> m_bandsIn;
  QtTransportInT<ModeList> m_modesIn;
  QtTransportOutT<RadioSettingsT, PayloadSource::SOURCE_FRONT_END> m_settingsOut;
  QtTransportFieldUpdateOut<PayloadSource::SOURCE_FRONT_END> m_fieldUpdateOut;
  bool m_waitingForSettings = false;
  bool m_waitingForModes = false;
  bool m_waitingForBands = false;
  QEventLoop* m_pWaitLoop = nullptr;
};