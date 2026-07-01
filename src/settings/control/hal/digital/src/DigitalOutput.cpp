//
// Created by murray on 25/11/25.
//

#include "CrossPlatformTypes.h"
#include <gpio/service/Gpio.h>
#include "settings/control/digital/DigitalOutputTypes.h"
#include "settings/control/digital/DigitalOutput.h"

#include "config/struct/DigitalOutputConfig.h"
#include "settings/model/core/RadioSettings.h"

DigitalOutput::DigitalOutput()
  : GpioLines(Direction::OUTPUT)
  , m_linesRequest(*this)
{
}

ResultCode
DigitalOutput::configure(const Config::DigitalOutput::Fields& config)
{
  ResultCode rc = GpioLines::configureLines(config);
  if (rc != ResultCode::OK) return rc;
  const Config::SettingPathString& strSettingPath = config.settingPath;
  // return RadioSettings::getSettingUpdatePath(strSettingPath, m_settingPath);
  bool isIndirectOut;
  AutoCompleteTrigger triggerOut;
  return RadioSettings::resolveDottedPath(strSettingPath.c_str(), m_settingPath, &isIndirectOut, &triggerOut);
}

ResultCode
DigitalOutput::applySettingUpdate(const SettingUpdate& setting)
{
  if (setting.path() == m_settingPath) {
    bool value = get<bool>(setting.value());
    setValue(value);
    return ResultCode::OK;
  }
  return ResultCode::ERR_SETTING_PATH_MISMATCH;
}

bool
DigitalOutput::discover()
{
  return Gpio::isPresent();
}

ResultCode
DigitalOutput::open()
{
  Gpio& gpio = Gpio::getInstance();

  return gpio.requestOutputs("digitalOutputs", &m_linesRequest);
}

void
DigitalOutput::close()
{

}

void
DigitalOutput::exit()
{

}

void
DigitalOutput::setValue(bool value)
{
  m_linesRequest.lineWriter(m_lines, value);
}
