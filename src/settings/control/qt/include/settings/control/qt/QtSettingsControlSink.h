#pragma once
#include <QObject>

#include "config/struct/QtControlSinkConfig.h"
#include "settings/control/sink/SettingsControlSink.h"

class QtSettingsControlSink : public QObject, public SettingsControlSink, public SettingUpdateSink
{
  Q_OBJECT
public:
  QtSettingsControlSink();
  QtSettingsControlSink(QtSettingsControlSink&& rhs) noexcept;
  ~QtSettingsControlSink() override = default;

  QtSettingsControlSink& operator=(QtSettingsControlSink&& rhs) noexcept;

  ResultCode configure(const Config::QtControlSink::Fields& config);

  bool discover() override;
  ResultCode open() override;
  void close() override;
  void exit() override;

  ResultCode applySettings(const RadioSettings& settings) override;
  ResultCode applySettingUpdate(const SettingUpdate& settingUpdate) override;

  void ptt(bool on) override {}

protected:
  uint64_t m_updateSequenceNo;

};