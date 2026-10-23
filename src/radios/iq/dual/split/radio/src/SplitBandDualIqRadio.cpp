#include "radios/iq/SplitBandDualIqRadio.h"

#ifdef IS_QT
#include <transport/qt/globalQtUpdateRings.h>

#include <transport/qt/globalQtIqRings.h>
#include <transport/qt/globalQtMetaRings.h>
#include <transport/qt/globalQtRxMeteringRings.h>
#include <transport/qt/globalQtSpectrumRings.h>
#endif

SplitBandDualIqRxTxRadio::SplitBandDualIqRxTxRadio(
  BandCategoryList& bands,
  ModeList& modes,
  SplitBandDualIqRxTxSettings::Cache& bandSettingsCache
  )
#ifdef IS_QT
  : m_transport(
    globalQtDualIqSplitRings(),
    globalQtUpdateRings(),
    globalQtMetaRings(),
    globalQtRxMeteringRings(),
    globalQtIqRings(),
    globalQtSpectrumRings()
    )
  , m_bands(bands)
#else
  : m_bands(bands)
#endif
  , m_modes(modes)
  , m_transceiver(bands, modes, &m_transport)
  , m_settings()
  , m_pUpdater(m_settings.updater())
{
  m_capabilitiesMsg.capabilities().setSettingsType(m_settings.getPayloadType());
  m_settings.setBands(&bands);
  m_settings.setModes(&modes);
  m_settings.setCache(&bandSettingsCache);
  m_settings.autoComplete();
}

ResultCode
SplitBandDualIqRxTxRadio::configure(const Config::Radio::Fields& config)
{
  if (!config.sdr) return ResultCode::ERR_CONFIG_NO_RXTX;

  const Config::Sdr::Fields& sdr = *config.sdr;

  m_capabilitiesMsg.capabilities().setFrequencyRanges(config.capabilities.frequencyRanges);

  ResultCode rc = m_transceiver.configure(sdr);
  if (rc != ResultCode::OK) return rc;

  rc = m_control.configure(config.control, m_settings.resolveDottedStringFunc());
  if (rc != ResultCode::OK) return rc;

  return m_transport.configure();
}

ResultCode
SplitBandDualIqRxTxRadio::start()
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
SplitBandDualIqRxTxRadio::stop()
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
SplitBandDualIqRxTxRadio::applyMessage(SplitBandDualIqRxTxSettings* settingsMessage)
{
  if (settingsMessage != nullptr) {
    return applySettings(*settingsMessage);
  }
  return ResultCode::OK;
}

ResultCode
SplitBandDualIqRxTxRadio::applyMessage(FieldUpdateMessage* updateMessage)
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
SplitBandDualIqRxTxRadio::applySettings(SplitBandDualIqRxTxSettings& settings)
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
SplitBandDualIqRxTxRadio::applyFieldUpdate(const FieldUpdate& update)
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
SplitBandDualIqRxTxRadio::processCurrentSettings()
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
SplitBandDualIqRxTxRadio::ptt(bool on)
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
SplitBandDualIqRxTxRadio::handleRequest(RadioRequester::Requests request)
{
  switch (request) {
  case RadioRequester::Requests::REQUEST_CAPABILITIES:
    return sendCapabilities();
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
SplitBandDualIqRxTxRadio::sendCapabilities()
{
  return m_transport.send(&m_capabilitiesMsg);
}

ResultCode
SplitBandDualIqRxTxRadio::sendSettings()
{
  m_settings.setPurpose(PayloadPurpose::PURPOSE_REPLACE);
  return m_transport.send(&m_settings);
}

ResultCode
SplitBandDualIqRxTxRadio::sendModes()
{
  return m_transport.send(&m_modes);
}

ResultCode
SplitBandDualIqRxTxRadio::sendBands()
{
  return m_transport.send(&m_bands);
}

