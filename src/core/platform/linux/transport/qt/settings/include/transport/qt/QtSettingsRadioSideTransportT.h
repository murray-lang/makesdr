#pragma once

#include <QThread>
#include <ResultCode.h>
#include <transport/qt/QtWakeT.h>
#include <settings/model/message/MessageSinkT.h>
#include <transport/RxMeteringPublisher.h>

#include <settings/model/update/FieldUpdateMessage.h>
#include <settings/model/meta/mode/ModeList.h>
#include <settings/model/meta/band/BandCategoryList.h>
#include <settings/model/radio/iq/FftMessage.h>

//*****************************************************************************
// The radio's transport on Qt: a RadioTransportT over the process-wide
// exchange, woken on the radio's own thread.
//
// The radio knows nothing of any client; it only sends into, and is woken by,
// the channels in globalQtMessageExchange().
//*****************************************************************************
template<typename RadioSettingsT>
class QtSettingsRadioSideTransportT
{

public:
  using Rings = SettingsRadioSideRingsT<QtSettingsMessageRingsT<RadioSettingsT>>;
  using RadioSettings = RadioSettingsT;

  QtSettingsRadioSideTransportT(QtSettingsMessageRingsT<RadioSettingsT>& globalRings)
    : m_rings(globalRings)
    , m_wake(m_rings)
  {
  }

  virtual ~QtSettingsRadioSideTransportT()
  {
    detach();
  }

  void moveToThread(QThread* thread) { m_wake.moveToThread(thread); }

  void connectRadioSettingsSink(MessageSinkT<RadioSettings>* settingsSink)
  {
    m_rings.connectRadioSettingsSink(settingsSink);
  }


  // Nothing to configure: the channels are fixed. Kept for existing callers.
  ResultCode configure() { return ResultCode::OK; }

  ResultCode attach()
  {
    m_rings.attach(&m_wake);
    return ResultCode::OK;
  }

  void detach() { m_rings.detach(); }

  ResultCode send(RadioSettings* settings)  { return m_rings.send(settings); }

protected:
  Rings         m_rings;
  QtWakeT<Rings>     m_wake;
};
