#include <CrossPlatformTypes.h>
#include "settings/control/digital/DigitalInput.h"

#include "settings/model/message/FieldUpdateSink.h"

DigitalInput::DigitalInput()
  : m_lineEventCallback(
    makeGpioLineEventCallback<DigitalInput, &DigitalInput::handleGpioLineEvent>(this)
    )
  , m_linesRequest(*this, m_lineEventCallback)
{
}

DigitalInput::DigitalInput(DigitalInput&& rhs)  noexcept
 : GpioInputLines(::move(static_cast<GpioInputLines&&>(rhs)))
  , m_lineEventCallback(makeGpioLineEventCallback<DigitalInput, &DigitalInput::handleGpioLineEvent>(this))
  , m_linesRequest(*this, m_lineEventCallback)
  , m_settingDescriptor(::move(rhs.m_settingDescriptor))
  , m_pSink(rhs.m_pSink)
{
  rhs.m_pSink = nullptr;
}

DigitalInput&
DigitalInput::operator=(DigitalInput&& rhs)  noexcept
{
  m_lineEventCallback = makeGpioLineEventCallback<DigitalInput, &DigitalInput::handleGpioLineEvent>(this);
  GpioInputLines::operator=(::move(static_cast<GpioInputLines&&>(rhs)));
  m_settingDescriptor = ::move(rhs.m_settingDescriptor);
  m_pSink = rhs.m_pSink;
  rhs.m_pSink = nullptr;
  return *this;
}

ResultCode
DigitalInput::configure(const Config::DigitalInput::Fields& config, ResolveDottedStringFunc resolver)
{
  ResultCode rc = configureLines(config);
  if (rc != ResultCode::OK) return rc;
  setEdge(Edge::BOTH);

  if (config.settingPath) {
    m_id = config.settingPath.value();
    return resolver(m_id.c_str(), m_settingDescriptor);
  } else if (config.settingDescriptor) {
    return m_settingDescriptor.configure(config.settingDescriptor.value());
  } else {
    return ResultCode::ERR_CONFIG_MISSING_SETTING_PATH;
  }
}

void
DigitalInput::handleGpioLineEvent(GpioLineEvent* event)
{
  if (isRotaryEncoder()) {
    auto value = static_cast<int32_t>(event->value);
    FieldUpdate setting(m_settingDescriptor, value, FieldUpdateMeaning::DELTA);
    notifyFieldUpdate(setting);
  } else {
    bool value = event->value > 0;
    FieldUpdate setting(m_settingDescriptor, value, FieldUpdateMeaning::VALUE);
    notifyFieldUpdate(setting);
  }
}

void
DigitalInput::connectFieldUpdateSink(FieldUpdateSink* pSink)
{
  m_pSink = pSink;
}

ResultCode
DigitalInput::notifyFieldUpdate(const FieldUpdate& settingUpdate)
{
  if (m_pSink) {
    m_pSink->applyFieldUpdate(settingUpdate);
  }
  return ResultCode::OK;
}
