#include "radios/iq/SingleIqRxRadio.h"
#ifdef IS_QT
#include <transport/qt/globalQtIqRings.h>
#include <transport/qt/globalQtMetaRings.h>
#endif

SingleIqRxRadio::SingleIqRxRadio(
  BandCategoryList& bands,
  ModeList& modes,
  SingleIqRxSettings::Cache& bandSettingsCache)
#ifdef IS_QT
: m_transport(globalQtSingleIqRxRings(), globalQtMetaRings(), globalQtIqRings())
, m_bands(bands)
#else
: m_bands(bands)
#endif
, m_modes(modes)
, m_receiver(m_pingPongBuffers, bands, modes, &m_transport)
, m_settings()
, m_pUpdater(m_settings.updater())
{
  m_settings.setBands(&bands);
  m_settings.setModes(&modes);
  m_settings.setCache(&bandSettingsCache);
}

ResultCode
SingleIqRxRadio::configure(const Config::Radio::Fields& config)
{
  if (!config.sdr) return ResultCode::ERR_CONFIG_NO_RXTX;

  const Config::Sdr::Fields& sdr = *config.sdr;
  if (!sdr.receiver) return ResultCode::ERR_CONFIG_RX_NO_RX;

  ResultCode rc = ResultCode::OK;
  if (sdr.receiver) {
    rc = m_receiver.configure(sdr.receiver.value());
    if (rc != ResultCode::OK) return rc;
  } else {
    return ResultCode::ERR_CONFIG_RX_NO_RX;
  }

  rc = m_control.configure(config.control, m_settings.resolveDottedStringFunc());
  if (rc != ResultCode::OK) return rc;

  return m_transport.configure();
}

ResultCode
SingleIqRxRadio::start()
{
  ResultCode rc = m_receiver.start();
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
SingleIqRxRadio::stop()
{
  m_transport.stop();
  m_transport.connectRadioSettingsSink(nullptr);
  m_transport.connectFieldUpdateSink(nullptr);
  m_control.stop();
  m_control.connectRadioSettingsSink(nullptr);
  m_control.connectFieldUpdateSink(nullptr);
  m_receiver.stop();
}

ResultCode
SingleIqRxRadio::applyMessage(Settings* settingsMessage)
{
  if (settingsMessage != nullptr) {
    return applySettings(*settingsMessage);
  }
  return ResultCode::OK;
}

ResultCode
SingleIqRxRadio::applyMessage(FieldUpdateMessage* updateMessage)
{
  if (updateMessage != nullptr) {
    FieldUpdate update(*updateMessage);
    if (update.isRequest()) {
      return handleRequest(static_cast<RadioRequester::Requests>(update.path().at(1)));
    } else {
      return applyFieldUpdate(update);
    }
  }
  return ResultCode::OK;
}

ResultCode
SingleIqRxRadio::applySettings(Settings& settings)
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
SingleIqRxRadio::applyFieldUpdate(const FieldUpdate& update)
{
  return ResultCode::OK; // TODO: Decide how to manage locally stored settings.
}

ResultCode
SingleIqRxRadio::ptt(bool on)
{
  if (on) {
    m_control.ptt(true);
    m_receiver.ptt(true);
  } else {
    m_receiver.ptt(true);
    m_control.ptt(true);
  }
  return ResultCode::OK;
}

ResultCode
SingleIqRxRadio::processCurrentSettings()
{
  // if (m_settings.hasPtt()) {
  //   ptt(m_settings.ptt());
  //   return ResultCode::OK;  // Don't try to do anything else concurrently with PTT.
  // }
  ResultCode rc = m_receiver.apply(m_settings);
  if (rc != ResultCode::OK) return rc;
  rc = m_control.applySettings(m_settings);
  if (rc != ResultCode::OK) return rc;
  rc = m_transport.send(&m_settings);

  // Clear all has_* flags so that only subsequent updates are seen as changes.
  m_settings.setAllFieldsPresence(false);
  return rc;
}

ResultCode
SingleIqRxRadio::handleRequest(RadioRequester::Requests request)
{
  switch (request) {
  case RadioRequester::Requests::REQUEST_SETTINGS:
    return sendSettings();
  case RadioRequester::Requests::REQUEST_MODES:
    return sendModes();
  case RadioRequester::Requests::REQUEST_BANDS:
    return sendBands();
  case RadioRequester::Requests::REQUEST_NONE:
    return ResultCode::OK;
  default:
    return ResultCode::ERR_RADIO_REQUEST_NOT_IMPLEMENTED;
  }
}

ResultCode
SingleIqRxRadio::sendSettings()
{
  m_settings.setPurpose(PayloadPurpose::PURPOSE_REPLACE);
  return m_transport.send(&m_settings);
}

ResultCode
SingleIqRxRadio::sendModes()
{
  return m_transport.send(&m_modes);
}

ResultCode
SingleIqRxRadio::sendBands()
{
  return m_transport.send(&m_bands);
}
