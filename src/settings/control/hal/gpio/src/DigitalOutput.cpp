//
// Created by murray on 25/11/25.
//

#include "CrossPlatformTypes.h"
#include "settings/control/gpio/DigitalOutputTypes.h"
#include "settings/control/gpio/DigitalOutput.h"

#include "settings/control/gpio/base/Gpio.h"
#include "config/struct/DigitalOutputConfig.h"
#include "settings/model/core/RadioSettings.h"

DigitalOutput::DigitalOutput() :
  GpioLines(Direction::OUTPUT)
{
}

ResultCode
DigitalOutput::configure(const Config::DigitalOutput::Fields& config)
{
  ResultCode rc = configureLines(config);
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
  if (m_linesRequest) {
    return ResultCode::OK;
  }
  Gpio& gpio = Gpio::getInstance();
  // DigitalOutputVariant thisAsVariant = move(*this);
  // DigitalOutputVariantVector thisInVector;
  // thisInVector.emplace_back(move(thisAsVariant));
  m_linesRequest.emplace();
  ResultCode rc = gpio.requestOutput("digitalOutputs", *this, *m_linesRequest);
  if (rc != ResultCode::OK) return rc;

  if (!m_linesRequest) {
    return ResultCode::ERR_SETTING_DIGITAL_OUTPUT_LINE_REQUEST_FAILED;
  }
  return ResultCode::OK;
}

void
DigitalOutput::close()
{
  if (m_linesRequest) {
    m_linesRequest->release();
    m_linesRequest.reset();
  }
}

void
DigitalOutput::exit()
{

}

void
DigitalOutput::setValue(bool value)
{
  m_linesRequest->setLineValue(m_lines.at(0), value);
}
