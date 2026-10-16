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
#include <transport/BasicClientSideRingsT.h>


#include "QtBasicMessageRings.h"


class QtBasicClientSideTransport : public FieldUpdateSink
{
public:
  using Rings = BasicClientSideRingsT<QtBasicMessageRings>;
  QtBasicClientSideTransport(QtBasicMessageRings& globalRings)
    : m_rings(globalRings)
    , m_wake(m_rings)
  {
  }

  virtual ~QtBasicClientSideTransport()
  {
    detach();
  }

  void moveToThread(QThread* thread) { m_wake.moveToThread(thread); }

  ResultCode applyFieldUpdate(const FieldUpdate& update) override
  {
    return m_rings.applyFieldUpdate(update);
  }

  void connectCapabilitiesSink(MessageSinkT<CapabilitiesMessage>* caps)
  {
    m_rings.connectCapabilitiesSink(caps);
  }

  void connectModesSink(MessageSinkT<ModeList>* modesSink)
  {
    m_rings.connectModesSink(modesSink);
  }

  void connectBandsSink(MessageSinkT<BandCategoryList>* sink)
  {
    m_rings.connectBandsSink(sink);
  }

  void connectRxMeteringSink(MessageSinkT<RxMeteringMessage>* sink)
  {
    m_rings.connectRxMeteringSink(sink);
  }

  // Nothing to configure: the channels are fixed. Kept for existing callers.
  ResultCode configure() { return ResultCode::OK; }

  ResultCode attach()
  {
    m_rings.attach(&m_wake);
    return ResultCode::OK;
  }

  void detach() { m_rings.detach(); }

protected:
  Rings          m_rings;
  QtWakeT<Rings> m_wake;
};
