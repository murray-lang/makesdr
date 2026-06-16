#include "RadioControlClient.h"

#include "../../data/exampleRadioSettings.h"

RadioControlClient::RadioControlClient()
  : m_radioSettings(exampleRadioSettingsPb)
{

}

ResultCode
RadioControlClient::applySettings(const RadioSettings& settings)
{
  return ResultCode::OK;
}
ResultCode
RadioControlClient::applySettingUpdate(const SettingUpdate& update)
{
  ResultCode rc = m_radioSettings.updateField(update);
  if (rc == ResultCode::OK) {
    rc = notifySettings(m_radioSettings);
  }
  return rc;
}