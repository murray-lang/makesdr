#include "radios/iq/SplitBandDualIqRadio.h"

SplitBandDualIqRadio::SplitBandDualIqRadio(
  BandCategoryList& bands,
  ModeList& modes,
  SplitBandDualIqRxTxSettings::Cache& bandSettingsCache
  )
  : m_bands(bands)
  , m_modes(modes)
  , m_transceiver(bands, modes)
  , m_settings()
  , m_pUpdater(m_settings.updater())
{
  m_settings.setBands(&bands);
  m_settings.setModes(&modes);
  m_settings.setCache(&bandSettingsCache);
  m_settings.autoComplete();
}

// SplitBandDualIqRadio::SplitBandDualIqRadio(EventTargetProvider eventTargetProvider, const RadioLookup& radioLookup,
//   SplitBandDualIqRxTxSettings::Cache& bandSettingsCache)
// {
// }

ResultCode
SplitBandDualIqRadio::configure(const Config::Radio::Fields& config)
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
SplitBandDualIqRadio::start()
{
  ResultCode rc = m_transceiver.start();
  if (rc != ResultCode::OK) return rc;
  m_control.connectRadioSettingsSink(this);
  m_control.connectFieldUpdateSink(this);
  rc = m_control.start();
  if (rc != ResultCode::OK) return rc;
  m_transport.connectRadioSettingsSink(this);
  m_transport.connectFieldUpdateSink(this);
  return m_transport.start();;
}

void
SplitBandDualIqRadio::stop()
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
SplitBandDualIqRadio::applyMessage(SplitBandDualIqRxTxSettings* settingsMessage)
{
  if (settingsMessage != nullptr) {
    return applySettings(*settingsMessage);
  }
  return ResultCode::OK;
}

ResultCode
SplitBandDualIqRadio::applyMessage(FieldUpdateMessage* updateMessage)
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
SplitBandDualIqRadio::applySettings(SplitBandDualIqRxTxSettings& settings)
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
SplitBandDualIqRadio::applyFieldUpdate(const FieldUpdate& update)
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
SplitBandDualIqRadio::processCurrentSettings()
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
SplitBandDualIqRadio::ptt(bool on)
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
SplitBandDualIqRadio::handleRequest(RadioSettingsRequester::Requests request)
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

ResultCode SplitBandDualIqRadio::sendSettings()
{
  m_settings.setPurpose(PayloadPurpose::PURPOSE_REPLACE);
  return m_transport.send(&m_settings);
}

ResultCode SplitBandDualIqRadio::sendModes()
{
  return m_transport.send(&m_modes);
}

ResultCode SplitBandDualIqRadio::sendBands()
{
  return m_transport.send(&m_bands);
}

