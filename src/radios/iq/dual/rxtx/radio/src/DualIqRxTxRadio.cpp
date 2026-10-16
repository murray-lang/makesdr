#include "radios/iq/DualIqRxTxRadio.h"
#include <transport/qt/globalQtDualIqRxTxRings.h>

DualIqRxTxRadio::DualIqRxTxRadio(
  BandCategoryList& bands,
  ModeList& modes,
  Settings::Cache& bandSettingsCache
  )
#ifdef IS_QT
  : m_transport(globalQtDualIqRxTxRings())
  , m_bands(bands)
#else
  : m_bands(bands)
#endif
  , m_modes(modes)
  , m_transceiver(bands, modes, &m_transport)
  , m_settings()
  , m_pUpdater(m_settings.updater())
{
  m_settings.setBands(&bands);
  m_settings.setModes(&modes);
  m_settings.setCache(&bandSettingsCache);
  m_settings.autoComplete();
}

ResultCode
DualIqRxTxRadio::configure(const Config::Radio::Fields& config)
{
  if (!config.sdr) return ResultCode::ERR_CONFIG_NO_RXTX;

  const Config::Sdr::Fields& sdr = *config.sdr;

  ResultCode rc = m_transceiver.configure(sdr);
  if (rc != ResultCode::OK) return rc;

  rc = m_control.configure(config.control, m_settings.resolveDottedStringFunc());
  if (rc != ResultCode::OK) return rc;

  return m_transport.configure();
}

ResultCode
DualIqRxTxRadio::start()
{
  ResultCode rc = m_transceiver.start();
  if (rc != ResultCode::OK) return rc;
  m_control.connectRadioSettingsSink(this);
  m_control.connectFieldUpdateSink(this);
  rc = m_control.start();
  if (rc != ResultCode::OK) return rc;
  m_transport.connectRadioSettingsSink(this);
  m_transport.connectFieldUpdateSink(this);
  rc = m_transport.start();
  if (rc != ResultCode::OK) return rc;
  m_settings.setPurpose(PayloadPurpose::PURPOSE_REPLACE);
  // m_settings.setAllFieldsPresence(true);
  return applySettings(m_settings);
}

void
DualIqRxTxRadio::stop()
{
  m_transport.stop();
  m_transport.connectRadioSettingsSink(nullptr);
  m_transport.connectFieldUpdateSink(nullptr);
  m_control.stop();
  m_control.connectRadioSettingsSink(nullptr);
  m_control.connectFieldUpdateSink(nullptr);
  m_transceiver.stop();
}

ResultCode
DualIqRxTxRadio::applyMessage(Settings* settingsMessage)
{
  if (settingsMessage != nullptr) {
    return applySettings(*settingsMessage);
  }
  return ResultCode::OK;
}

ResultCode
DualIqRxTxRadio::applyMessage(FieldUpdateMessage* updateMessage)
{
  if (updateMessage != nullptr) {
    FieldUpdate update(*updateMessage);
    if (update.isRequest()) {
      return handleRequest(static_cast<RadioSettingsRequester::Requests>(update.path().at(1)));
    } else {
      return applyFieldUpdate(update);
    }
  }
  return ResultCode::OK;
}

ResultCode
DualIqRxTxRadio::applySettings(Settings& settings)
{
  ResultCode rc = ResultCode::OK;
  m_settingsMutex.lock();
  if (settings.purpose() == PayloadPurpose::PURPOSE_REPLACE) {
    rc = m_settings.replace(settings, true);
  } else if (settings.purpose() == PayloadPurpose::PURPOSE_MERGE) {
    rc = m_settings.merge(settings);
  } else {
    rc = ResultCode::OK;
  }
  m_settingsMutex.unlock();
  if (rc != ResultCode::OK) return rc;

  return processCurrentSettings();
}

ResultCode
DualIqRxTxRadio::applyFieldUpdate(const FieldUpdate& update)
{
  m_settingsMutex.lock();
  ResultCode rc = m_settings.applyFieldUpdate(update);
  m_settingsMutex.unlock();
  if (rc != ResultCode::OK) return rc;
  if (update.isFinal()) {
    return processCurrentSettings();
  }
  return ResultCode::OK;
}

// ResultCode
// SplitBandDualIqRadio::applyFieldUpdate(const FieldUpdate& update, PayloadSource source)
// {
//   ResultCode rc = m_settings.applyFieldUpdate(update);
//   if (rc != ResultCode::OK) return rc;
//   if (update.isFinal()) {
//     return sendSettings();
//   }
//   return ResultCode::OK;
// }

ResultCode
DualIqRxTxRadio::processCurrentSettings()
{
  // if (m_settings.hasPtt()) {
  //   ptt(m_settings.ptt());
  //   return ResultCode::OK;  // Don't try to do anything else concurrently with PTT.
  // }
  ResultCode rc = m_transceiver.apply(m_settings);
  if (rc != ResultCode::OK) return rc;
  rc = m_control.applySettings(m_settings);
  if (rc != ResultCode::OK) return rc;
  rc = m_transport.send(&m_settings);

  // Clear all has_* flags so that only subsequent updates are seen as changes.
  m_settings.setAllFieldsPresence(false);
  return rc;
}

ResultCode
DualIqRxTxRadio::ptt(bool on)
{
  if (on) {
    m_control.ptt(true);
    m_transceiver.ptt(true);
  } else {
    m_transceiver.ptt(true);
    m_control.ptt(true);
  }
  return ResultCode::OK;
}

ResultCode
DualIqRxTxRadio::handleRequest(RadioSettingsRequester::Requests request)
{
  switch (request) {
  case RadioSettingsRequester::Requests::REQUEST_SETTINGS:
    return sendSettings();
  case RadioSettingsRequester::Requests::REQUEST_MODES:
    return sendModes();
  case RadioSettingsRequester::Requests::REQUEST_BANDS:
    return sendBands();
  case RadioSettingsRequester::Requests::REQUEST_NONE:
    return ResultCode::OK;
  default:
    return ResultCode::ERR_RADIO_REQUEST_NOT_IMPLEMENTED;
  }
}

ResultCode DualIqRxTxRadio::sendSettings()
{
  m_settings.setPurpose(PayloadPurpose::PURPOSE_REPLACE);
  return m_transport.send(&m_settings);
}

ResultCode DualIqRxTxRadio::sendModes()
{
  return m_transport.send(&m_modes);
}

ResultCode DualIqRxTxRadio::sendBands()
{
  return m_transport.send(&m_bands);
}

