#include "settings/control/qt/QtSettingsControlSink.h"
#include "settings/control/qt/QtGlobalControlEventTargets.h"

#include <qcoreapplication.h>


// #include "core/config-settings/settings/events/RadioSettingsEvent.h"
// #include "core/config-settings/settings/events/SettingUpdateEvent.h"


QtSettingsControlSink::QtSettingsControlSink()
  : m_updateSequenceNo(0)
{
}

QtSettingsControlSink::QtSettingsControlSink(QtSettingsControlSink&& rhs) noexcept
  : m_updateSequenceNo(0)
{}

QtSettingsControlSink&
QtSettingsControlSink::operator=(QtSettingsControlSink&& rhs) noexcept
{
  return *this;
}

ResultCode
QtSettingsControlSink::configure(const Config::QtControlSink::Fields& config)
{
  return ResultCode::OK;
}

bool
QtSettingsControlSink::discover()
{
  return true;
}

ResultCode
QtSettingsControlSink::open()
{
  return ResultCode::OK;
}

void
QtSettingsControlSink::close()
{
}

void
QtSettingsControlSink::exit()
{

}

ResultCode
QtSettingsControlSink::applySettings(const RadioSettings& settings)
{
  // if (globalControlClientEventTarget != nullptr) {
  //   auto* rse = new RadioSettingsEvent(settings, ++m_updateSequenceNo, SettingEventBase::BACK_END);
  //   QCoreApplication::postEvent(globalControlClientEventTarget, rse);
  // }
  return ResultCode::OK;
}

ResultCode
QtSettingsControlSink::applySettingUpdate(const SettingUpdate& settingUpdate)
{
  // if (globalControlClientEventTarget != nullptr) {
  //   auto* sue = new SettingUpdateEvent(settingDelta, SettingEventBase::BACK_END);
  //   QCoreApplication::postEvent(globalControlClientEventTarget, sue);
  // }
  return ResultCode::OK;
}

