//
// Created by murray on 20/8/25.
//

#include "settings/control/gpio/RotaryEncoder.h"
// #include <qdebug.h>

#include "settings/model/core/SettingUpdate.h"

RotaryEncoder::RotaryEncoder() : DigitalInput()
{
  m_detectEdge = true;
}

ResultCode
RotaryEncoder::configure(const Config::RotaryEncoder::Fields& config)
{
  setEdge(Edge::BOTH);
  return DigitalInput::configure(config);
}

void
RotaryEncoder::notifyMovement(const int movement)
{
  SettingUpdate setting(m_settingPath, movement, SettingUpdate::DELTA, m_isPathIndirect, m_autoCompleteTrigger);
  notifySettingUpdate(setting);
}

bool
RotaryEncoder::handleLineChange(DigitalInputLinesRequest::LineStateVector& changedLines)
{
  DigitalInputLinesRequest::LineState& a = changedLines.at(m_lines[0]);
  DigitalInputLinesRequest::LineState& b = changedLines.at(m_lines[1]);
  if (!a.changed || !b.changed) {
    return false;
  }
  if (m_debounce && (!a.isDebounced || !b.isDebounced)) {
    return false;
  }
  int dir = calculateMovement(a, b);
  // qDebug() << "A:" << a.value << "B:" << b.value << "Direction: " << dir;
  notifyMovement(dir);
  a.changed = false; // TODO: Encapsulate this in LineState
  b.changed = false; // TODO: Encapsulate this in LineState
  a.isDebounced = false;
  b.isDebounced = false;
  a.firstEdgeTime = 0;
  b.firstEdgeTime = 0;
  return true;

 
}

int
RotaryEncoder::calculateMovement(DigitalInputLinesRequest::LineState& a, DigitalInputLinesRequest::LineState& b)
{
  if (a.value ==  1 && b.value == 1) {
    return a.lastRisingTime < b.lastRisingTime ? +1 : -1;
  }
  if (a.value == 0 && b.value == 0) {
    return a.lastFallingTime < b.lastFallingTime ? +1 : -1;
  }
  if (a.value == 1 && b.value == 0) {
    return a.lastRisingTime < b.lastFallingTime ? -1 : +1;
  }
  if (a.value == 0 && b.value == 1) {
    return a.lastFallingTime < b.lastRisingTime ? -1 : +1;
  }
  return 0;

}
