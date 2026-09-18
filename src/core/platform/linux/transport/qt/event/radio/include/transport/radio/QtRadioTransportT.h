#pragma once
#include <QThread>
#include <transport/in-out/QtTransportInOutT.h>
#include <settings/model/message/FieldUpdateMessage.h>
#include <settings/model/data/mode/ModeList.h>
#include <settings/model/data/band/BandCategoryList.h>


template<typename RadioSettingsT>
class QtRadioTransportT
{
public:
  QtRadioTransportT()
    : m_thread()
    , m_settingsTransport(m_thread)
    , m_updateTransport(m_thread)
    , m_modesTransport(m_thread)
    , m_bandsTransport(m_thread)
  {
  }
  // Must join m_thread before it is destroyed, otherwise ~QThread calls qFatal()
  // ("Destroyed while thread is still running") on any path that skips stop().
  ~QtRadioTransportT()
  {
    stop();
  }

  void connectRadioSettingsSink(MessageSinkT<RadioSettingsT>* settingsSink)
  {
    m_settingsTransport.connectMessageInSink(settingsSink);
  }

  void connectFieldUpdateSink(MessageSinkT<FieldUpdateMessage>* updateSink)
  {
    m_updateTransport.connectMessageInSink(updateSink);
  }

  ResultCode configure()
  {
    QtTransportIoConfig settingsConfig  {
      .in =  { .message = Message::Settings, .target = Target::Radio },
      .out = { .message = Message::Settings, .target = Target::Client }
    };
    ResultCode rc = m_settingsTransport.configure(settingsConfig);
    if (rc != ResultCode::OK) return rc;

    QtTransportIoConfig updateConfig  {
      .in =  { .message = Message::Update, .target = Target::Radio },
      .out = { .message = Message::None, .target = Target::Client }
    };
    rc = m_updateTransport.configure(updateConfig);
    if (rc != ResultCode::OK) return rc;

    QtTransportIoConfig modesConfig  {
      .in =  { .message = Message::None, .target = Target::Radio },
      .out = { .message = Message::Modes, .target = Target::Client }
    };
    rc = m_modesTransport.configure(modesConfig);
    if (rc != ResultCode::OK) return rc;

    QtTransportIoConfig bandsConfig  {
      .in =  { .message = Message::None, .target = Target::Radio },
      .out = { .message = Message::Bands, .target = Target::Client }
    };
    rc = m_bandsTransport.configure(bandsConfig);
    if (rc != ResultCode::OK) return rc;
    return ResultCode::OK;
  }

  ResultCode start()
  {
    ResultCode rc = m_settingsTransport.start();
    if (rc != ResultCode::OK) return rc;
    rc = m_updateTransport.start();
    if (rc != ResultCode::OK) return rc;
    rc = m_modesTransport.start();
    if (rc != ResultCode::OK) return rc;
    rc = m_bandsTransport.start();
    if (rc != ResultCode::OK) return rc;
    m_thread.start();
    return ResultCode::OK;
  }

  void stop()
  {
    m_settingsTransport.stop();
    m_updateTransport.stop();
    m_modesTransport.stop();
    m_bandsTransport.stop();
    m_thread.quit();
    m_thread.wait();
  }

  ResultCode send(RadioSettingsT* settings)
  {
    return m_settingsTransport.send(settings);
  }

  ResultCode send(ModeList* modes)
  {
    return m_modesTransport.send(modes);
  }

  ResultCode send(BandCategoryList* bands)
  {
    return m_bandsTransport.send(bands);
  }

private:
  QThread m_thread;
  QtTransportInOutT<RadioSettingsT, PayloadSource::SOURCE_BACK_END> m_settingsTransport;
  QtTransportInOutT<FieldUpdateMessage, PayloadSource::SOURCE_BACK_END> m_updateTransport;
  QtTransportInOutT<ModeList, PayloadSource::SOURCE_BACK_END> m_modesTransport;
  QtTransportInOutT<BandCategoryList, PayloadSource::SOURCE_BACK_END> m_bandsTransport;
};
