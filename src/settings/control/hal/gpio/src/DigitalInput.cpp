#include "gpio/DigitalInput.h"
#include "settings/MutableRadioSettings.h"

#include "settings/SettingFieldUpdateSink.h"

DigitalInput::DigitalInput() :
  GpioInputLines(),
  m_detectEdge(false)
{
}

ResultCode
DigitalInput::configure(const Config::DigitalInput::Fields& config)
{
  ResultCode rc = GpioLines::configureLines(config);
  if (rc != ResultCode::OK) return rc;

  m_activeHigh = config.activeHigh;
  m_debounce = config.debounce;
  const Config::SettingPathString& strSettingPath = config.settingPath;
  m_id = strSettingPath;
  setEdge(Edge::BOTH);
  bool isIndirectOut;
  AutoCompleteTrigger triggerOut;
  return MutableRadioSettings::resolveDottedPath(strSettingPath.c_str(), m_settingPath, &isIndirectOut, &triggerOut);
}

void
DigitalInput::connectSettingFieldUpdateSink(SettingFieldUpdateSink& pSink)
{
  m_pSink.emplace(pSink);
}
ResultCode
DigitalInput::notifySettingFieldUpdate(const SettingFieldUpdate& settingUpdate)
{
  if (m_pSink) {
    m_pSink->get().applySettingFieldUpdate(settingUpdate); //.applySettingFieldUpdate(settingDelta);
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
  SettingFieldUpdate setting(m_settingPath, static_cast<bool>(active), SettingFieldUpdate::VALUE);
  notifySettingFieldUpdate(setting);
}
