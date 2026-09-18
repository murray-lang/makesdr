#pragma once
#include <QThread>

#include <config/struct/QtControlSourceConfig.h>
#include <settings/control/source/SettingsControlSourceT.h>
#include <event/FieldUpdateEvent.h>
#include <EventId.h>
#include <qeventloop.h>


template <typename RadioSettingsT>
class QtSettingsControlSourceT : public SettingsControlSourceT<RadioSettingsT>, public QThread
{
public:
  QtSettingsControlSourceT() = default;
  QtSettingsControlSourceT(QtSettingsControlSourceT&& rhs) noexcept
    : SettingsControlSourceT<RadioSettingsT>(move(rhs))
  {
  }
  ~QtSettingsControlSourceT() override
  {
    QtSettingsControlSourceT::close();
  }

  QtSettingsControlSourceT& operator=(QtSettingsControlSourceT&& rhs) noexcept
  {
    SettingsControlSourceT<RadioSettingsT>::operator=(move(rhs));
    return *this;
  }

  ResultCode configure(const Config::QtTransportIn::Fields& config) { return ResultCode::OK; }

  bool discover() override { return true; }

  ResultCode open() override
  {
    globalControlRadioEventTarget = this;
    start();
    return ResultCode::OK;
  }
  void close() override
  {
    globalControlRadioEventTarget = nullptr;
    // emit finished();
    wait();
  }

  void exit() override {}

  void run() override
  {
    QEventLoop loop;
    // QObject::connect(this, &QtTransportIn::finished, &loop, &QEventLoop::quit);
    loop.exec();
  }

  void customEvent(QEvent* event) override
  {
    // if (event->type() == RadioSettingsEvent::RadioSettingsEventType) {
    //
    //   auto* radioSettingsEvent = dynamic_cast<RadioSettingsEvent*>(event);
    //   notifySettings(radioSettingsEvent->getRadioSettings());
    //
    // } else
    if (event->type() == static_cast<QEvent::Type>(EVENT_SETTINGS_UPDATE)) {

      auto* settingUpdateEvent = dynamic_cast<FieldUpdateEvent*>(event);
      // notifyFieldUpdate(settingUpdateEvent->m_setting);
    }
  }

  // signals:
  //   void finished();

protected:

};
