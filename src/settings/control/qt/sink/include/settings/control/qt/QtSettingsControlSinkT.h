#pragma once

#include <config/struct/QtControlSinkConfig.h>
#include <settings/control/sink/SettingsControlSinkT.h>
#include <settings/model/radios/RadioSettingsEventT.h>
#include <settings/control/qt/QtGlobalControlEventTargets.h>
#include <settings/control/qt/QtSettingsControlSinkBase.h>
#include <event/QtEventRegistrar.h>

#include "event/EventDispatcher.h"

template<typename RadioSettingsT>
class QtSettingsControlSinkT : public QtSettingsControlSinkBase, public SettingsControlSinkT<RadioSettingsT>, public FieldUpdateSink
{
public:
  using RadioSettingsEvent = RadioSettingsEventT<RadioSettingsT, QEvent, QEvent::Type, RadioSettingsT::eventId>;

  QtSettingsControlSinkT()
    : m_updateSequenceNo(0)
  {
  }

  QtSettingsControlSinkT(QtSettingsControlSinkT&& rhs) noexcept
    : m_updateSequenceNo(0)
  {
  }

  ~QtSettingsControlSinkT() override = default;

  QtSettingsControlSinkT& operator=(QtSettingsControlSinkT&& rhs) noexcept { return *this; }

  ResultCode configure(const Config::QtTransportOut::Fields& config) { return ResultCode::OK; }

  bool discover() override { return true; }
  ResultCode open() override { return ResultCode::OK; }
  void close() override {}
  void exit() override {}

  ResultCode applySettings(RadioSettingsT& settings) override
  {
    if (globalControlClientEventTarget != nullptr) {
      auto* rse = new RadioSettingsEvent(settings, ++m_updateSequenceNo, RadioSettingsEvent::BACK_END);
      EventDispatcher::dispatch<RadioSettingsEvent>(globalControlClientEventTarget, rse);
      // QCoreApplication::postEvent(globalControlClientEventTarget, rse);
    }
    return ResultCode::OK;
  }

  ResultCode applyFieldUpdate(const FieldUpdate& settingUpdate) override
  {
    // if (globalControlClientEventTarget != nullptr) {
    //   auto* sue = new FieldUpdateEvent(settingDelta, SettingEventBase::BACK_END);
    //   QCoreApplication::postEvent(globalControlClientEventTarget, sue);
    // }
    return ResultCode::OK;
  }

  ResultCode ptt(bool on) override { return ResultCode::OK; }

protected:
  uint64_t m_updateSequenceNo;

};
