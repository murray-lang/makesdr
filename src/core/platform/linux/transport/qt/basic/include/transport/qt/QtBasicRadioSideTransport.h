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

#include "QtBasicMessageRings.h"

//*****************************************************************************
// The radio's transport on Qt: a RadioTransportT over the process-wide
// exchange, woken on the radio's own thread.
//
// The radio knows nothing of any client; it only sends into, and is woken by,
// the channels in globalQtMessageExchange().
//*****************************************************************************
class QtBasicRadioSideTransport
{

public:
  // using Rings = typename RingsT::Rings;
  using Rings = BasicRadioSideRingsT<QtBasicMessageRings>;

  QtBasicRadioSideTransport(QtBasicMessageRings& globalRings)
    : m_rings(globalRings)
    , m_wake(m_rings)
  {
  }

  virtual ~QtBasicRadioSideTransport()
  {
    detach();
  }

  void moveToThread(QThread* thread) { m_wake.moveToThread(thread); }

  RxMeteringPublisher* getRxMeteringPublisher() { return m_rings.getRxMeteringPublisher(); }


  void connectFieldUpdateSink(MessageSinkT<FieldUpdateMessage>* updateSink)
  {
    m_rings.connectFieldUpdateSink(updateSink);
  }

  // Nothing to configure: the channels are fixed. Kept for existing callers.
  ResultCode configure() { return ResultCode::OK; }

  ResultCode attach()
  {
    m_rings.attach(&m_wake);
    return ResultCode::OK;
  }

  void detach() { m_rings.detach(); }

  ResultCode send(CapabilitiesMessage* caps)   { return m_rings.send(caps); }
  ResultCode send(ModeList* modes)             { return m_rings.send(modes); }
  ResultCode send(BandCategoryList* bands)     { return m_rings.send(bands); }
  ResultCode send(RxMeteringMessage* metering) { return m_rings.send(metering); }

protected:
  Rings          m_rings;
  QtWakeT<Rings> m_wake;
};
