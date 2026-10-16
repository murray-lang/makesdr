#pragma once

#include "QtSplitBandDualIqMessageRings.h"
#include <transport/IqRadioSideRingsT.h>

#include <transport/qt/QtBasicRadioSideTransport.h>
#include <transport/qt/QtIqRadioSideTransport.h>
#include <transport/SettingsRadioSideRingsT.h>

#include <transport/qt/QtBasicMessageRings.h>
#include <transport/qt/QtIqMessageRings.h>
#include <transport/qt/QtSettingsRadioSideTransportT.h>



using SettingsRings = SettingsRadioSideRingsT<QtSplitBandDualIqMessageRings>;
using IqRings = IqRadioSideRingsT<QtIqMessageRings>;

// using SplitBandDualIqTransport = QtIqRadioSideTransportT<Rings>;

class SplitBandDualIqTransport : public IRadioPublishers
{
public:
  SplitBandDualIqTransport(
    SettingsRings::Rings& settingsRings,
    QtBasicMessageRings& basicRings,
    IqRings::Rings& iqRings
    )
    : m_thread()
    , m_settingsTransport(settingsRings)
    , m_basicTransport(basicRings)
    , m_iqTransport(iqRings)
  {
    m_settingsTransport.moveToThread(&m_thread);
    m_basicTransport.moveToThread(&m_thread);
  }

  ~SplitBandDualIqTransport() override
  {
    stop();
  }

  ResultCode configure()
  {
    ResultCode rc = m_basicTransport.configure();
    if (rc != ResultCode::OK) return rc;
    rc = m_settingsTransport.configure();
    if (rc != ResultCode::OK) return rc;
    return m_iqTransport.configure();
  }

  ResultCode start()
  {
    ResultCode rc = m_basicTransport.attach();
    if (rc != ResultCode::OK) return rc;
    rc = m_settingsTransport.attach();
    if (rc != ResultCode::OK) return rc;

    m_thread.start();
    return ResultCode::OK;
  }

  void stop()
  {
    m_settingsTransport.detach();
    m_basicTransport.detach();
    m_thread.quit();
    m_thread.wait();
  }

  void connectRadioSettingsSink(MessageSinkT<SettingsRings::RadioSettings>* settingsSink)
  {
    m_settingsTransport.connectRadioSettingsSink(settingsSink);
  }

  void connectFieldUpdateSink(MessageSinkT<FieldUpdateMessage>* updateSink)
  {
    m_basicTransport.connectFieldUpdateSink(updateSink);
  }

  RxMeteringPublisher* getRxMeteringPublisher() override { return m_basicTransport.getRxMeteringPublisher(); }
  IqPublisher* getIqPublisher() override { return m_iqTransport.getIqPublisher(); }
  FftPublisher* getFftPublisher() override { return m_iqTransport.getFftPublisher(); }


  ResultCode send(CapabilitiesMessage* caps) { return m_basicTransport.send(caps); }
  ResultCode send(SettingsRings::RadioSettings* settings)  { return m_settingsTransport.send(settings); }
  ResultCode send(ModeList* modes)           { return m_basicTransport.send(modes); }
  ResultCode send(BandCategoryList* bands)   { return m_basicTransport.send(bands); }

private:
  // Declaration order matters: the transports' wakes live in m_thread, so
  // they are destroyed before it.
  QThread m_thread;
  QtSettingsRadioSideTransportT<SplitBandDualIqRxTxSettings> m_settingsTransport;
  QtBasicRadioSideTransport m_basicTransport;
  QtIqRadioSideTransport m_iqTransport;

};
