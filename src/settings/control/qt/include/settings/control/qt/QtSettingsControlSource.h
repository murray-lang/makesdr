#pragma once
#include <QThread>

#include "config/struct/QtControlSourceConfig.h"
#include "settings/control/source/SettingsControlSource.h"

class QtSettingsControlSource : public SettingsControlSource, public QThread
{
public:
  QtSettingsControlSource();
  QtSettingsControlSource(QtSettingsControlSource&& rhs) noexcept;
  ~QtSettingsControlSource() override;

  QtSettingsControlSource& operator=(QtSettingsControlSource&& rhs) noexcept;

  ResultCode configure(const Config::QtControlSource::Fields& config);

  bool discover() override;
  ResultCode open() override;
  void close() override;
  void exit() override;

  void run() override;

  void customEvent(QEvent* event) override;

  // signals:
  //   void finished();

protected:

};
