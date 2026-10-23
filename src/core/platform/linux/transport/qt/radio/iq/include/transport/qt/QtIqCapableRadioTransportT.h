#pragma once

#include <QThread>

#include <transport/IRadioPublishers.h>
#include <transport/IqRadioSideRingsT.h>

#include <transport/qt/QtMetaRadioSideTransport.h>
#include <transport/qt/QtIqRadioSideTransport.h>

#include <transport/qt/QtUpdateMessageRings.h>
#include <transport/qt/QtMetaMessageRings.h>
#include <transport/qt/QtIqMessageRings.h>
#include <transport/qt/QtSettingsRadioSideTransportT.h>

#include <transport/qt/QtRxMeteringRadioSideTransport.h>
#include <transport/qt/QtSpectrumRadioSideTransport.h>

#include <transport/qt/QtUpdateRadioSideTransport.h>


template<typename SettingsRingsT>
class QtIqCapableRadioTransportT : public IRadioPublishers
{
public:
  QtIqCapableRadioTransportT(
    SettingsRingsT& settingsRings,
    QtUpdateMessageRings& updateRings,
    QtMetaMessageRings& metaRings,
    QtRxMeteringMessageRings& rxMeteringRings,
    QtIqMessageRings& iqRings,
    QtSpectrumMessageRings& spectrumRings
    )
    : m_thread()
    , m_settingsTransport(settingsRings)
    , m_updateTransport(updateRings)
    , m_metaTransport(metaRings)
    , m_rxMeteringTransport(rxMeteringRings)
    // , m_iqTransport(iqRings)
    , m_spectrumTransport(spectrumRings)
  {
    m_settingsTransport.recipient().moveToThread(&m_thread);
  }

  ~QtIqCapableRadioTransportT() override
  {
    stop();
  }

  // Nothing to configure: the channels are fixed.
  ResultCode configure() { return ResultCode::OK; }

  // Only settings travel to the radio, so only that side is woken.
  ResultCode start()
  {
    m_settingsTransport.attachRecipient();
    m_updateTransport.attachRecipient();
    m_thread.start();
    return ResultCode::OK;
  }

  void stop()
  {
    m_settingsTransport.detachRecipient();
    m_updateTransport.detachRecipient();
    m_thread.quit();
    m_thread.wait();
  }

  void connectRadioSettingsSink(MessageSinkT<typename SettingsRingsT::RadioSettings>* settingsSink)
  {
    m_settingsTransport.connectRadioSettingsSink(settingsSink);
  }

  void connectFieldUpdateSink(MessageSinkT<FieldUpdateMessage>* updateSink)
  {
    m_updateTransport.connectFieldUpdateSink(updateSink);
  }

  RxMeteringPublisher* getRxMeteringPublisher() override { return m_rxMeteringTransport.getRxMeteringPublisher(); }
  // IqPublisher* getIqPublisher() override { return m_iqTransport.getIqPublisher(); }
  IqPublisher* getIqPublisher() override { return nullptr; }
  SpectrumPublisher* getSpectrumPublisher() override { return m_spectrumTransport.getSpectrumPublisher(); }


  ResultCode send(CapabilitiesMessage* caps) { return m_metaTransport.send(caps); }
  ResultCode send(SettingsRingsT::RadioSettings* settings)  { return m_settingsTransport.send(settings); }
  ResultCode send(ModeList* modes)           { return m_metaTransport.send(modes); }
  ResultCode send(BandCategoryList* bands)   { return m_metaTransport.send(bands); }

private:
  // Declaration order matters: the transports' recipients live in m_thread, so
  // they are destroyed before it.
  QThread m_thread;

  // QtIqRadioSideTransport m_iqTransport;
  QtUpdateRadioSideTransport m_updateTransport;
  QtMetaRadioSideTransport m_metaTransport;
  QtRxMeteringRadioSideTransport m_rxMeteringTransport;
  QtSettingsRadioSideTransportT<typename SettingsRingsT::RadioSettings> m_settingsTransport;
  QtSpectrumRadioSideTransport m_spectrumTransport;
};
