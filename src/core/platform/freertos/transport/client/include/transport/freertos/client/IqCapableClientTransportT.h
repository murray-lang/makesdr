#pragma once

#include <ResultCode.h>
#include <thread/Runnable.h>
#include <thread/Thread.h>

#include <transport/RecipientSideT.h>
#include <transport/MetaRadioSideRingsT.h>
#include <transport/RxMeteringRadioSideRingsT.h>
#include <transport/SettingsRadioSideRingsT.h>
#include <transport/SpectrumRadioSideRingsT.h>
#include <transport/UpdateRadioSideRingsT.h>
#include <transport/freertos/TaskDispatcher.h>
#include <transport/freertos/TaskRecipientT.h>
#include <settings/model/radio/RxMeteringMessage.h>

#include "transport/MetaClientSideRingsT.h"
#include "transport/RxMeteringClientSideRingsT.h"
#include "transport/SettingsClientSideRingsT.h"
#include "transport/SpectrumClientSideRingsT.h"
#include "transport/UpdateClientSideRingsT.h"

#ifndef CLIENT_TRANSPORT_STACK_SIZE
#define CLIENT_TRANSPORT_STACK_SIZE 1024
#endif

#ifndef CLIENT_TRANSPORT_PRIORITY
#define CLIENT_TRANSPORT_PRIORITY 3
#endif

//*****************************************************************************
// The radio's end of the rings shared with the CM7: the counterpart of
// QtIqCapableRadioTransportT, with the same interface, so a radio selects one
// or the other by platform and is otherwise unchanged.
//
// Where the Qt transport moves its settings recipient to a QThread, this one
// delivers incoming messages to the connected sinks on a FreeRTOS task that
// waits on its TaskDispatcher. That task is either:
//
//  - its own: start()/stop() run a "ClientTransport" task that does nothing
//    else; or
//  - hosted: the client's task (e.g. the LVGL task, so the settings mirror,
//    the client and the display share one thread) calls attach() once, then
//    waitAndDispatch() in its loop in place of its delay, and detach() when
//    done.
//
// Use one or the other, not both.
//
// There are no IQ rings between the cores (the UI takes spectrum messages),
// so getIqPublisher() returns nullptr, as the Qt transport's currently does.
//*****************************************************************************

template<
  typename SettingsRingsT,
  typename UpdateRingsT,
  typename MetaRingsT,
  typename RxMeteringRingsT,
  typename SpectrumRingsT
>
class IqCapableClientTransportT : private Runnable
{
public:
  using SettingsRings = SettingsRingsT;
  using RadioSettings = SettingsRings::RadioSettings;
  using SettingsTransport = RecipientSideT<SettingsClientSideRingsT<SettingsRingsT>, TaskRecipientT>;
  using UpdateTransport = UpdateClientSideRingsT<UpdateRingsT>;

  IqCapableClientTransportT(
    SettingsRingsT& settingsRings,
    UpdateRingsT& updateRings,
    MetaRingsT& metaRings,
    RxMeteringRingsT& rxMeteringRings,
    SpectrumRingsT& spectrumRings
    )
    : m_thread(*this)
    , m_settingsTransport(settingsRings, m_dispatcher)
    , m_updateTransport(updateRings)
    , m_metaTransport(metaRings, m_dispatcher)
    , m_rxMeteringTransport(rxMeteringRings, m_dispatcher)
    , m_spectrumTransport(spectrumRings, m_dispatcher)
  {
  }

  ~IqCapableClientTransportT() override
  {
    stop();
  }

  // Nothing to configure: the channels are fixed.
  ResultCode configure() { return ResultCode::OK; }

  // Only settings and field updates travel to the radio, so only those sides
  // are woken; the task attaches them once it is running.
  ResultCode start()
  {
    if (m_thread.isRunning()) {
      return ResultCode::OK;
    }
    configASSERT(!m_attached);  // already hosted by another task
    const bool started = m_thread.start(
      CLIENT_TRANSPORT_STACK_SIZE/sizeof(uint32_t), CLIENT_TRANSPORT_PRIORITY, "ClientTransport");
    return started ? ResultCode::OK : ResultCode::ERR_TRANSPORT_THREAD_START_FAILED;
  }

  void stop()
  {
    if (!m_thread.isRunning()) {
      return;
    }
    // Wake the task so it notices the stop request without waiting for its
    // timeout.
    if (TaskHandle_t task = m_dispatcher.taskHandle()) {
      TaskNotify::setBits(task, 0);
    }
    m_thread.stop();
  }

  //--- hosted mode: all on the hosting task ---------------------------------

  // Makes the calling task the one that delivers incoming messages to the
  // sinks. Call before anything is requested, so no wake is missed.
  void attach()
  {
    if (m_attached) {
      return;
    }
    m_dispatcher.bindToCurrentTask();
    m_settingsTransport.attachRecipient();
    m_metaTransport.attachRecipient();
    m_rxMeteringTransport.attachRecipient();
    m_spectrumTransport.attachRecipient();
    m_attached = true;
  }

  void detach()
  {
    if (!m_attached) {
      return;
    }
    m_settingsTransport.detachRecipient();
    m_metaTransport.detachRecipient();
    m_rxMeteringTransport.detachRecipient();
    m_spectrumTransport.detachRecipient();
    m_attached = false;
  }

  // Blocks until messages are waiting or timeout, then delivers them to the
  // sinks. Returns true if anything was waiting.
  bool waitAndDispatch(TickType_t timeout)
  {
    return m_dispatcher.waitAndDispatch(timeout);
  }

  void connectRadioSettingsSink(MessageSinkT<RadioSettings>* settingsSink)
  {
    m_settingsTransport.connectRadioSettingsSink(settingsSink);
  }

  void connectRxMeteringSink(MessageSinkT<RxMeteringMessage>* meteringSink)
  {
    m_rxMeteringTransport.connectRxMeteringSink(meteringSink);
  }

  void connectCapabilitiesSink(MessageSinkT<CapabilitiesMessage>* caps)
  {
    m_metaTransport.connectCapabilitiesSink(caps);
  }

  void connectModesSink(MessageSinkT<ModeList>* sink)
  {
    m_metaTransport.connectModesSink(sink);
  }

  void connectBandsSink(MessageSinkT<BandCategoryList>* sink)
  {
    m_metaTransport.connectBandsSink(sink);
  }

  void connectSpectrumSink(MessageSinkT<SpectrumMessage>* sink)
  {
    m_spectrumTransport.connectSpectrumSink(sink);
  }

  ResultCode send(FieldUpdateMessage* update)   { return m_updateTransport.send(update); }

  SettingsTransport& getSettingsTransport() { return m_settingsTransport; }
  UpdateTransport& getUpdateTransport() { return m_updateTransport; }

  // The task that delivers incoming messages to the connected sinks. Null
  // until the transport task has started or a task has attached. Callers that block waiting for a
  // reply must not do so on this task.
  [[nodiscard]] TaskHandle_t dispatcherTask() const { return m_dispatcher.taskHandle(); }

private:
  void run() override
  {
    attach();
    while (!m_thread.shouldStop()) {
      waitAndDispatch(pdMS_TO_TICKS(WaitTimeoutMs));
    }
    detach();
  }

  // A backstop only: stop() wakes the task directly.
  static constexpr uint32_t WaitTimeoutMs = 100;

  // Declaration order matters: the dispatcher is constructed before the
  // recipients that register with it, and destroyed after them.
  Thread         m_thread;
  TaskDispatcher m_dispatcher;

  SettingsTransport                                                            m_settingsTransport;
  UpdateTransport                                                              m_updateTransport;
  RecipientSideT<MetaClientSideRingsT<MetaRingsT>, TaskRecipientT>             m_metaTransport;
  RecipientSideT<RxMeteringClientSideRingsT<RxMeteringRingsT>, TaskRecipientT> m_rxMeteringTransport;
  RecipientSideT<SpectrumClientSideRingsT<SpectrumRingsT>, TaskRecipientT>     m_spectrumTransport;

  bool m_attached = false;   // the dispatching task only
};
