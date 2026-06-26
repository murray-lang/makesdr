#include "settings/control/gpio/DigitalInput.h"
#include "settings/model/core/RadioSettings.h"

#include "settings/model/core/SettingUpdateSink.h"

DigitalInput::DigitalInput() :
  GpioInputLinesConfig(),
  m_detectEdge(false),
  m_isPathIndirect(false),
  m_autoCompleteTrigger(AutoCompleteTrigger::NONE)
{
}

ResultCode
DigitalInput::configure(const Config::DigitalInput::Fields& config)
{
  ResultCode rc = GpioLinesConfig::configureLines(config);
  if (rc != ResultCode::OK) return rc;

  m_activeHigh = config.activeHigh;
  m_debounce = config.debounce;
  const Config::SettingPathString& strSettingPath = config.settingPath;
  m_id = strSettingPath;
  setEdge(Edge::BOTH);
  return RadioSettings::resolveDottedPath(
    strSettingPath.c_str(),
    m_settingPath,
    &m_isPathIndirect,
    &m_autoCompleteTrigger
    );
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
    m_pSink->get().applySettingUpdate(settingUpdate); //.applySettingUpdate(settingDelta);
  }
  return ResultCode::OK;
}

bool 
DigitalInput::handleLineChange(DigitalInputLinesRequest::LineStateVector& changedLines)
{
  DigitalInputLinesRequest::LineState& line = changedLines.at(m_lines[0]);
  if (!line.changed) {
    return false;
  }
  if (m_debounce && !line.isDebounced) {
    return false;
  }
 
  // qDebug() << "A:" << a.value << "B:" << b.value << "Direction: " << dir;
  notifyChange(line);
  line.changed = false; // TODO: Encapsulate this in LineState
  line.isDebounced = false;
  line.firstEdgeTime = 0;
  return true;
}

void
DigitalInput::notifyChange(const DigitalInputLinesRequest::LineState& lineState)
{
  bool active = m_activeHigh ? !!lineState.value : !lineState.value;
  SettingUpdate setting(m_settingPath, active, SettingUpdate::VALUE, m_isPathIndirect, m_autoCompleteTrigger);
  notifySettingUpdate(setting);
}
