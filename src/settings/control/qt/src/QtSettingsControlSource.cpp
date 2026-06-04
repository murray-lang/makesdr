#include "../include/settings/control/qt/QtSettingsControlSource.h"
#include "../include/settings/control/qt/QtGlobalControlEventTargets.h"

#include <qeventloop.h>

// #include "core/config-settings/settings/events/RadioSettingsEvent.h"
// #include "core/config-settings/settings/events/SettingUpdateEvent.h"


QtSettingsControlSource::QtSettingsControlSource()
{
}

QtSettingsControlSource::QtSettingsControlSource(QtSettingsControlSource&& rhs) noexcept
  : SettingsControlSource(move(rhs))
{
}

QtSettingsControlSource::~QtSettingsControlSource()
{
  QtSettingsControlSource::close();
}

QtSettingsControlSource&
QtSettingsControlSource::operator=(QtSettingsControlSource&& rhs) noexcept
{
  SettingsControlSource::operator=(move(rhs));
  return *this;
}

ResultCode
QtSettingsControlSource::configure(const Config::QtControlSource::Fields& config)
{
  return ResultCode::OK;
}

bool
QtSettingsControlSource::discover()
{
  return true;
}

ResultCode
QtSettingsControlSource::open()
{
  globalControlRadioEventTarget = this;
  start();
  return ResultCode::OK;
}

void
QtSettingsControlSource::close()
{
  globalControlRadioEventTarget = nullptr;
  // emit finished();
  wait();
}

void
QtSettingsControlSource::exit()
{

}

void
QtSettingsControlSource::run()
{
  QEventLoop loop;
  // QObject::connect(this, &QtControlSource::finished, &loop, &QEventLoop::quit);
  loop.exec();
}

void
QtSettingsControlSource::customEvent(QEvent* event)
{
  // if (event->type() == RadioSettingsEvent::RadioSettingsEventType) {
  //
  //   auto* radioSettingsEvent = dynamic_cast<RadioSettingsEvent*>(event);
  //   notifySettings(radioSettingsEvent->getRadioSettings());
  //
  // } else if (event->type() == SettingUpdateEvent::SettingUpdateEventType) {
  //
  //   auto* settingUpdateEvent = dynamic_cast<SettingUpdateEvent*>(event);
  //   notifySettingUpdate(settingUpdateEvent->m_setting);
  // }
}