#pragma once
#include <type_traits>

#include <QThread>
#include <ResultCode.h>
#include <transport/radio/RadioTransportT.h>
#include <transport/qt/event/QtWakeT.h>
#include <transport/qt/event/globalQtMessageExchange.h>

//*****************************************************************************
// The radio's transport on Qt: a RadioTransportT over the process-wide
// exchange, woken on the radio's own thread.
//
// The radio knows nothing of any client; it only sends into, and is woken by,
// the channels in globalQtMessageExchange().
//*****************************************************************************
template<typename RadioSettingsT>
class QtRadioTransportT
{
  static_assert(std::is_same_v<RadioSettingsT, QtRadioMessageExchange::Settings>,
                "The radio's settings type must match the build's RadioSettings");

public:
  QtRadioTransportT()
    : m_thread()
    , m_transport(globalQtMessageExchange())
    , m_wake(m_transport)
  {
    // Before the thread starts, so events are handled there from the outset.
    m_wake.moveToThread(&m_thread);
  }

  // Must join m_thread before it is destroyed, otherwise ~QThread calls qFatal()
  // ("Destroyed while thread is still running") on any path that skips stop().
  ~QtRadioTransportT()
  {
    stop();
  }

  void connectRadioSettingsSink(MessageSinkT<RadioSettingsT>* settingsSink)
  {
    m_transport.connectRadioSettingsSink(settingsSink);
  }

  void connectFieldUpdateSink(MessageSinkT<FieldUpdateMessage>* updateSink)
  {
    m_transport.connectFieldUpdateSink(updateSink);
  }

  // Nothing to configure: the channels are fixed. Kept for existing callers.
  ResultCode configure() { return ResultCode::OK; }

  ResultCode start()
  {
    m_transport.attach(&m_wake);
    m_thread.start();
    return ResultCode::OK;
  }

  void stop()
  {
    m_transport.detach();
    m_thread.quit();
    m_thread.wait();
  }

  IqPublisher* getIqPublisher() { return m_transport.getIqPublisher(); }

  ResultCode send(RadioSettingsT* settings)  { return m_transport.send(settings); }
  ResultCode send(IqMessage* iq)             { return m_transport.send(iq); }
  ResultCode send(ModeList* modes)           { return m_transport.send(modes); }
  ResultCode send(BandCategoryList* bands)   { return m_transport.send(bands); }

private:
  using Transport = RadioTransportT<QtRadioMessageExchange>;

  // Declaration order matters: m_wake refers to m_transport and lives in
  // m_thread, so it is destroyed first.
  QThread                m_thread;
  Transport              m_transport;
  QtWakeT<Transport>     m_wake;
};
