#pragma once

#include <ResultCode.h>
#include <thread/Runnable.h>
#include <thread/Thread.h>

#include <transport/IRadioPublishers.h>
#include <transport/RecipientSideT.h>
#include <transport/MetaRadioSideRingsT.h>
#include <transport/RxMeteringRadioSideRingsT.h>
#include <transport/SettingsRadioSideRingsT.h>
#include <transport/SpectrumRadioSideRingsT.h>
#include <transport/UpdateRadioSideRingsT.h>
#include <transport/freertos/TaskDispatcher.h>
#include <transport/freertos/TaskRecipientT.h>

#ifndef RADIO_TRANSPORT_STACK_SIZE
#define RADIO_TRANSPORT_STACK_SIZE 1024
#endif

#ifndef RADIO_TRANSPORT_PRIORITY
#define RADIO_TRANSPORT_PRIORITY 3
#endif

//*****************************************************************************
// The radio's end of the rings shared with the CM7: the counterpart of
// QtIqCapableRadioTransportT, with the same interface, so a radio selects one
// or the other by platform and is otherwise unchanged.
//
// Where the Qt transport moves its settings recipient to a QThread, this one
// runs a FreeRTOS task that waits on its TaskDispatcher and delivers settings
// and field updates from the CM7 to the connected sinks, on that task.
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
class IqCapableRadioTransportT : public IRadioPublishers, private Runnable
{
public:
  using RadioSettings = SettingsRingsT::RadioSettings;

  IqCapableRadioTransportT(
    SettingsRingsT& settingsRings,
    UpdateRingsT& updateRings,
    MetaRingsT& metaRings,
    RxMeteringRingsT& rxMeteringRings,
    SpectrumRingsT& spectrumRings
    )
    : m_thread(*this)
    , m_settingsTransport(settingsRings, m_dispatcher)
    , m_updateTransport(updateRings, m_dispatcher)
    , m_metaTransport(metaRings)
    , m_rxMeteringTransport(rxMeteringRings)
    , m_spectrumTransport(spectrumRings)
  {
  }

  ~IqCapableRadioTransportT() override
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
    const bool started = m_thread.start(
      RADIO_TRANSPORT_STACK_SIZE/sizeof(uint32_t), RADIO_TRANSPORT_PRIORITY, "RadioTransport");
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

  void connectRadioSettingsSink(MessageSinkT<RadioSettings>* settingsSink)
  {
    m_settingsTransport.connectRadioSettingsSink(settingsSink);
  }

  void connectFieldUpdateSink(MessageSinkT<FieldUpdateMessage>* updateSink)
  {
    m_updateTransport.connectFieldUpdateSink(updateSink);
  }

  RxMeteringPublisher* getRxMeteringPublisher() override { return m_rxMeteringTransport.getRxMeteringPublisher(); }
  IqPublisher* getIqPublisher() override { return nullptr; }
  SpectrumPublisher* getSpectrumPublisher() override { return m_spectrumTransport.getSpectrumPublisher(); }

  ResultCode send(CapabilitiesMessage* caps) { return m_metaTransport.send(caps); }
  ResultCode send(RadioSettings* settings)   { return m_settingsTransport.send(settings); }
  ResultCode send(ModeList* modes)           { return m_metaTransport.send(modes); }
  ResultCode send(BandCategoryList* bands)   { return m_metaTransport.send(bands); }

private:
  void run() override
  {
    m_dispatcher.bindToCurrentTask();
    m_settingsTransport.attachRecipient();
    m_updateTransport.attachRecipient();

    while (!m_thread.shouldStop()) {
      m_dispatcher.waitAndDispatch(pdMS_TO_TICKS(WaitTimeoutMs));
    }

    m_settingsTransport.detachRecipient();
    m_updateTransport.detachRecipient();
  }

  // A backstop only: stop() wakes the task directly.
  static constexpr uint32_t WaitTimeoutMs = 100;

  // Declaration order matters: the dispatcher is constructed before the
  // recipients that register with it, and destroyed after them.
  Thread         m_thread;
  TaskDispatcher m_dispatcher;

  RecipientSideT<SettingsRadioSideRingsT<SettingsRingsT>, TaskRecipientT>            m_settingsTransport;
  RecipientSideT<UpdateRadioSideRingsT<UpdateRingsT>, TaskRecipientT> m_updateTransport;
  MetaRadioSideRingsT<MetaRingsT>                                     m_metaTransport;
  RxMeteringRadioSideRingsT<RxMeteringRingsT>                         m_rxMeteringTransport;
  SpectrumRadioSideRingsT<SpectrumRingsT>                             m_spectrumTransport;
};
