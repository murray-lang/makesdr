#include "stm32h745i/client/RadioClient.h"

#include <cstdio>


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
  , m_pSettingsListener(nullptr)
{
  // Settings go to m_settingsMirror; spectrum and metering to whatever the
  // application connects.
  m_transport.connectCapabilitiesSink(this);
  m_transport.connectBandsSink(this);
  m_transport.connectModesSink(this);

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
  m_transport.attach();
  return ResultCode::OK;
}

void RadioClient::stop()
{
  m_awaiting = Awaiting::None;
  m_transport.detach();
}

bool
RadioClient::waitAndDispatch(TickType_t timeout)
{
  const bool dispatched = m_transport.waitAndDispatch(timeout);

  if (m_awaiting != Awaiting::None
      && (xTaskGetTickCount() - m_requestSentAt) >= pdMS_TO_TICKS(RequestTimeoutMs)) {
    failRequestAll(ResultCode::ERR_RADIO_REQUEST_TIMEOUT);
  }
  return dispatched;
}

ResultCode
RadioClient::requestAll()
{
  m_requestAllStatus = ResultCode::OK;
  // Capabilities aren't requested yet: start with modes
  return request(Awaiting::Modes);
}

ResultCode
RadioClient::request(Awaiting what)
{
  // Set before sending, so that a send failure reports what was awaited.
  // The reply can't arrive before this returns: it is dispatched on this task.
  m_awaiting = what;
  m_requestSentAt = xTaskGetTickCount();

  ResultCode rc = ResultCode::OK;
  switch (what) {
    case Awaiting::Capabilities: rc = requestCapabilities(); break;
    case Awaiting::Modes:        rc = requestModes(); break;
    case Awaiting::Bands:        rc = requestBands(); break;
    // The reply goes to m_settingsMirror, which calls applySettings() and so notifyReceived(Settings)
    case Awaiting::Settings:     rc = requestSettings(); break;
    case Awaiting::None:         break;
  }
  if (rc != ResultCode::OK) {
    failRequestAll(rc);
  }
  return rc;
}

void
RadioClient::failRequestAll(ResultCode rc)
{
  printf("requestAll() failed awaiting %d (%d)\r\n", static_cast<int>(m_awaiting), static_cast<int>(rc));
  m_awaiting = Awaiting::None;
  m_requestAllStatus = rc;
}

// Called from applyMessage()/applySettings() on the hosting task. Unsolicited
// messages (e.g. settings pushed by the radio) don't advance the sequence.
void
RadioClient::notifyReceived(Awaiting what)
{
  if (what != m_awaiting) {
    return;
  }
  switch (what) {
    case Awaiting::Modes:    request(Awaiting::Bands); break;
    case Awaiting::Bands:    request(Awaiting::Settings); break;
    default:                 m_awaiting = Awaiting::None; break;
  }
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
