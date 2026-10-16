#pragma once

#include <settings/model/radio/iq/IqMessage.h>
#include <settings/model/radio/iq/FftMessage.h>

#include <transport/BasicMessageRingsT.h>


#define IQ_RING_SIZE 1
#define FFT_RING_SIZE 1


//*****************************************************************************
// BasicMessageRingsT plus the IQ and FFT rings, both radio -> client.
//
// Each pipe carries messages one way only, since a ring has exactly one
// producer. The client attaches to all of its rings at once, and the payload
// type passed to the notifier identifies which pipe to read.
//*****************************************************************************
template <typename NotifierT>
class IqMessageRingsT //: public BasicMessageRingsT<RadioSettingsT, NotifierT>
{
  // using Base = BasicMessageRingsT<RadioSettingsT, NotifierT>;

public:
  using IqRing       = MessageRingT<IqMessage, IQ_RING_SIZE, NotifierT>;
  using FftRing      = MessageRingT<FftMessage, FFT_RING_SIZE, NotifierT>;

  //--- radio -> client -------------------------------------------------------

  IqRing&       iqToClient()       { return m_iqToClient; }
  FftRing&      fftToClient()      { return m_fftToClient; }

  //--- attaching consumers (only where NotifierT supports it) ----------------
  //
  // These hide, rather than override, the base versions: member templates
  // cannot be virtual, and callers always hold the concrete rings type.
  // attachRadio()/detachRadio() are inherited unchanged.

  template<typename TargetT>
  void attachClient(TargetT target)
  {
    // Base::attachClient(target);

    m_iqToClient.notifier().attach(target);
    m_fftToClient.notifier().attach(target);

    m_iqToClient.wakeIfPending();
    m_fftToClient.wakeIfPending();
  }

  void detachClient()
  {
    // Base::detachClient();

    m_iqToClient.notifier().detach();
    m_fftToClient.notifier().detach();
  }

protected:
  IqRing       m_iqToClient;
  FftRing      m_fftToClient;
};
