#include "settings/control/digital/DigitalInput.h"
#include "settings/model/core/RadioSettings.h"

#include "settings/model/core/SettingUpdateSink.h"

// #include <stm32h745i/app/support/safe_printf.h>

DigitalInput::DigitalInput()
  : m_lineEventCallback(
    GpioLineEventCallback::create<DigitalInput, &DigitalInput::handleGpioLineEvent>(*this)
    )
  , m_linesRequest(*this, m_lineEventCallback)
  , m_isPathIndirect(false)
  , m_autoCompleteTrigger(AutoCompleteTrigger::NONE)
{
}

DigitalInput::DigitalInput(DigitalInput&& rhs)  noexcept
 : GpioInputLines(move(rhs))
  , m_lineEventCallback(GpioLineEventCallback::create<DigitalInput, &DigitalInput::handleGpioLineEvent>(*this))
  , m_linesRequest(*this, m_lineEventCallback)
  , m_settingPath(move(rhs.m_settingPath))
  , m_isPathIndirect(rhs.m_isPathIndirect)
  , m_autoCompleteTrigger(rhs.m_autoCompleteTrigger)
  , m_pSink(move(rhs.m_pSink))
{

}

DigitalInput&
DigitalInput::operator=(DigitalInput&& rhs)  noexcept
{
  m_lineEventCallback = GpioLineEventCallback::create<DigitalInput, &DigitalInput::handleGpioLineEvent>(*this);
  GpioInputLines::operator=(move(rhs));
  m_isPathIndirect = rhs.m_isPathIndirect;
  m_settingPath = move(rhs.m_settingPath);
  m_autoCompleteTrigger = rhs.m_autoCompleteTrigger;
  m_pSink = move(rhs.m_pSink);
  return *this;
}

ResultCode
DigitalInput::configure(const Config::DigitalInput::Fields& config)
{
  ResultCode rc = configureLines(config);
  if (rc != ResultCode::OK) return rc;

  const Config::SettingPathString& strSettingPath = config.settingPath;
  m_id = strSettingPath;
  setEdge(Edge::BOTH);
  rc = RadioSettings::resolveDottedPath(
    strSettingPath.c_str(),
    m_settingPath,
    &m_isPathIndirect,
    &m_autoCompleteTrigger
    );
  // printf("[CM4]\tDigitalInput::configure() SettingPath: %lu, %lu, %lu, %lu, %lu, %lu\r\n", m_settingPath[0], m_settingPath[1], m_settingPath[2], m_settingPath[3], m_settingPath[4], m_settingPath[5]);
  return rc;
}

void
DigitalInput::handleGpioLineEvent(GpioLineEvent* event)
{
  if (isRotaryEncoder()) {
    auto value = static_cast<int32_t>(event->value);
    SettingUpdate setting(m_settingPath, value, SettingUpdate::DELTA, m_isPathIndirect, m_autoCompleteTrigger);
    notifySettingUpdate(setting);
  } else {
    bool value = event->value > 0;
    SettingUpdate setting(m_settingPath, value, SettingUpdate::VALUE, m_isPathIndirect, m_autoCompleteTrigger);
    notifySettingUpdate(setting);
  }
}

void
DigitalInput::connectSettingUpdateSink(SettingUpdateSink& pSink)
{
  m_pSink.emplace(pSink);
}

ResultCode
DigitalInput::notifySettingUpdate(const SettingUpdate& settingUpdate)
{
  if (m_pSink) {
    m_pSink->get().applySettingUpdate(settingUpdate);
  }
  return ResultCode::OK;
}
