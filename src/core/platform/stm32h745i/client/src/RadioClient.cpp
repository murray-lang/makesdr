#include "stm32h745i/client/RadioClient.h"


RadioClient::RadioClient()
  : m_transport(
    globalStm32H745SingleIqRxRings(),
    globalStm32H745UpdateRings(),
    globalStm32H745MetaRings(),
    globalStm32H745RxMeteringRings(),
    globalStm32H745SpectrumRings()
    )
  , m_settingsMirror(m_transport, this)
  , m_pUpdater(m_settingsMirror.updater())
{
  // m_transport.connectRadioSettingsSink(this);
  m_transport.connectSpectrumSink(this);
  m_transport.connectCapabilitiesSink(this);
  m_transport.connectBandsSink(this);
  m_transport.connectModesSink(this);
  m_transport.connectRxMeteringSink(this);

  m_requester.connectFieldUpdateSink(&m_transport.getUpdateTransport());
}

ResultCode
RadioClient::configure(const Config::Radio::Fields& config)
{
  return ResultCode::OK;
}

ResultCode
RadioClient::applyFieldUpdate(const FieldUpdate& update)
{
  return ResultCode::OK;
}

ResultCode
RadioClient::start()
{
  return m_transport.start();
}

void RadioClient::stop()
{
  m_transport.stop();
}

// ResultCode
// RadioClient::applyMessage(RadioSettings* message)
// {
//   return ResultCode::OK;
//
// }

ResultCode
RadioClient::requestAll()
{
  // ResultCode rc = requestAndWait(
  //   Awaiting::Capabilities,
  //   &RadioClient::requestCapabilities,
  //   1000
  // );
  // if (rc != ResultCode::OK) {
  //   printf("Failed to request capabilities (%d)\r\n", rc);
  //   return rc;
  // }

  ResultCode rc = requestAndWait(
    Awaiting::Modes,
    &RadioClient::requestModes,
    2000
  );
  if (rc != ResultCode::OK) {
    printf("Failed to request modes (%d)\r\n", rc);
    return rc;
  }

  rc = requestAndWait(
    Awaiting::Bands,
    &RadioClient::requestBands,
    2000
  );
  if (rc != ResultCode::OK) {
    printf("Failed to request bands (%d)\r\n", rc);
    return rc;
  }

  // The reply goes to m_settingsMirror, which calls applySettings() and so notifyReceived(Settings)
  rc = requestAndWait(
    Awaiting::Settings,
    &RadioClient::requestSettings,
    2000
  );
  if (rc != ResultCode::OK) {
    printf("Failed to request settings (%d)\r\n", rc);
    return rc;
  }

  return ResultCode::OK;
}

ResultCode RadioClient::applyMessage(CapabilitiesMessage* message)
{
  notifyReceived(Awaiting::Capabilities);
  return ResultCode::OK;
}

ResultCode
RadioClient::applyMessage(BandCategoryList* message)
{
  if (message != nullptr) {
    m_bands = *message;
  }
  notifyReceived(Awaiting::Bands);
  return ResultCode::OK;
}

ResultCode
RadioClient::applyMessage(ModeList* message)
{
  if (message != nullptr) {
    m_modes = *message;
  }
  notifyReceived(Awaiting::Modes);
  return ResultCode::OK;
}

ResultCode
RadioClient::applyMessage(RxMeteringMessage* message)
{
  return ResultCode::OK;
}

ResultCode
RadioClient::applyMessage(SpectrumMessage* message)
{
  // BSP_LED_Toggle(LED_RED);
  return ResultCode::OK;
}

ResultCode
RadioClient::requestAndWait(Awaiting what, ResultCode (RadioClient::*requestFunc)(), uint32_t timeoutMs)
{
  // Must not block the task that delivers the reply
  if (xTaskGetCurrentTaskHandle() == m_transport.dispatcherTask()) {
    return ResultCode::ERR_RADIO_REQUEST_WOULD_DEADLOCK;
  }
  while (m_replied.tryWait()) {}   // drain a late post from an earlier timed-out request

  m_awaiting.store(what);
  ResultCode rc = (this->*requestFunc)();
  if (rc != ResultCode::OK) {
    m_awaiting.store(Awaiting::None);
    return rc;
  }
  const bool received = m_replied.waitFor(timeoutMs);
  m_awaiting.store(Awaiting::None);
  return received ? ResultCode::OK : ResultCode::ERR_RADIO_REQUEST_TIMEOUT;
}

// Called from applyMessage()/applySettings() on the transport task
void
RadioClient::notifyReceived(Awaiting what)
{
  Awaiting expected = what;
  if (m_awaiting.compare_exchange_strong(expected, Awaiting::None)) {
    m_replied.post();
  }
}