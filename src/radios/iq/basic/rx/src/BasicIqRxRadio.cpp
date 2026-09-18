#include "radios/iq/BasicIqRxRadio.h"

BasicIqRxRadio::BasicIqRxRadio(
  const BandCategoryList& bands,
  const ModeList& modes,
  BasicIqRxSettings::Cache& bandSettingsCache)
  : m_settings()
  , m_receiver(modes)
{
  m_settings.setBands(&bands);
  m_settings.setModes(&modes);
  m_settings.setCache(&bandSettingsCache);
}

ResultCode
BasicIqRxRadio::configure(const Config::Radio::Fields& config)
{
  if (!config.sdr) return ResultCode::ERR_CONFIG_NO_RXTX;

  const Config::Sdr::Fields& sdr = *config.sdr;
  if (!sdr.receiver) return ResultCode::ERR_CONFIG_RXTX_NO_RX;

  ResultCode rc = m_receiver.configure(*sdr.receiver);
  if (rc != ResultCode::OK) return rc;

  return m_control.configure(config.control);
}

ResultCode
BasicIqRxRadio::start()
{
  ResultCode rc = m_receiver.start();
  if (rc != ResultCode::OK) return rc;
  m_control.connectRadioSettingsSink(this);
  m_control.connectFieldUpdateSink(this);
  return m_control.start();
}

void
BasicIqRxRadio::stop()
{
  m_control.stop();
  m_control.connectRadioSettingsSink(nullptr);
  m_control.connectFieldUpdateSink(nullptr);
  m_receiver.stop();
}

ResultCode
BasicIqRxRadio::applySettings(BasicIqRxSettings& settings)
{
  m_settings.merge(settings.body());

  if (settings.hasPtt()) {
    ptt(settings.ptt());
    return ResultCode::OK;  // Don't try to do anything else concurrently with PTT.
  }
  ResultCode rc = m_receiver.apply(m_settings);
  if (rc != ResultCode::OK) return rc;
  return m_control.applySettings(settings);

}

ResultCode
BasicIqRxRadio::applyFieldUpdate(const FieldUpdate& update)
{
  return ResultCode::OK; // TODO: Decide how to manage locally stored settings.
}

ResultCode
BasicIqRxRadio::ptt(bool on)
{
  if (on) {
    pttOn();
  } else {
    pttOff();
  }
  return ResultCode::OK;
}

void
BasicIqRxRadio::pttOn()
{
  m_receiver.stop();
  m_control.ptt(true);
}

void
BasicIqRxRadio::pttOff()
{
  m_control.ptt(true);
  m_receiver.start();
}