
#include "linux/test-utils/QtTransportTestRadio.h"
#include <settings/model/data/band/mostBandCategories.h>
#include <settings/model/data/mode/basicModes.h>
#include <test-utils/testRadioSettings.h>

using Target = Config::QtTransport::Target;
using Message = Config::QtTransport::Message;

struct TransportIoConfig
{
  Config::QtTransportIn::Fields in;
  Config::QtTransportOut::Fields out;
};

BandCategoryList bandsByCategory(mostBandCategories);
ModeList modeList(basicModes);
SplitBandDualIqRxTxSettings::Cache bandSettingsCache;


QtTransportTestRadio::QtTransportTestRadio()
  : m_transport()
  , m_settings(testRadioSettingsPayloadPb)
  , m_pShortcuts(m_settings.updater())
{
  m_settings.setBands(&bandsByCategory);
  m_settings.setModes(&modeList);
  m_settings.setCache(&bandSettingsCache);
  m_settings.autoComplete();
  m_transport.connectRadioSettingsSink(this);
  m_transport.connectFieldUpdateSink(this);
}

QtTransportTestRadio::~QtTransportTestRadio()
{
}

ResultCode
QtTransportTestRadio::configure()
{
  return m_transport.configure();
}

ResultCode
QtTransportTestRadio::configure(const Config::Radio::Fields& config)
{
  return configure();
}

ResultCode
QtTransportTestRadio::start()
{
 return m_transport.start();
}
void
QtTransportTestRadio::stop()
{
  m_transport.stop();
}

ResultCode
QtTransportTestRadio::applyMessage(SplitBandDualIqRxTxSettings* settingsMessage)
{
  if (settingsMessage != nullptr) {
    return applySettings(*settingsMessage);
  }
  return ResultCode::OK;
}

ResultCode
QtTransportTestRadio::applyMessage(FieldUpdateMessage* updateMessage)
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
QtTransportTestRadio::applySettings(SplitBandDualIqRxTxSettings& settings)
{
  if (settings.purpose() == PayloadPurpose::PURPOSE_REPLACE) {
    return m_settings.replace(settings, true);
  }
  if (settings.purpose() == PayloadPurpose::PURPOSE_MERGE) {
    return m_settings.merge(settings);
  }
  return ResultCode::OK;
}

ResultCode
QtTransportTestRadio::applyFieldUpdate(const FieldUpdate& update)
{
  ResultCode rc = m_settings.applyFieldUpdate(update);
  if (rc != ResultCode::OK) return rc;
  if (update.isFinal()) {
    return sendSettings();
  }
  return ResultCode::OK;
}

ResultCode
QtTransportTestRadio::ptt(bool on)
{
  if (m_pShortcuts != nullptr) {
    return m_pShortcuts->ptt(on);
  }
  return ResultCode::OK;
}

ResultCode
QtTransportTestRadio::handleRequest(RadioSettingsRequester::Requests request)
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

ResultCode QtTransportTestRadio::sendSettings()
{
  m_settings.setPurpose(PayloadPurpose::PURPOSE_REPLACE);
  return m_transport.send(&m_settings);
}

ResultCode QtTransportTestRadio::sendModes()
{
  return m_transport.send(&modeList);
}

ResultCode QtTransportTestRadio::sendBands()
{
  return m_transport.send(&bandsByCategory);
}
