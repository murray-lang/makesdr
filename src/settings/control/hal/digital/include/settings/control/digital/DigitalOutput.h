#pragma once

#include <CrossPlatformTypes.h>
#include "config/struct/DigitalOutputConfig.h"
#include "gpio/base/GpioLines.h"
#include "gpio/service/GpioOutputLinesSource.h"
#include "settings/control/sink/SettingsControlSink.h"
#include "settings/model/core/RadioSettingsSink.h"
#include "settings/model/core/SettingPath.h"
#include "settings/model/core/SettingUpdate.h"
#include "settings/model/core/SettingUpdateSink.h"

#ifdef USE_ETL
#include "etl/memory.h"
using etl::unique_ptr;
#else
#include <memory>
using std::unique_ptr;
#endif


class DigitalOutput : public GpioLines, public SettingsControlSink, public SettingUpdateSink
{
public:
  DigitalOutput();
  ~DigitalOutput() override = default;

  DigitalOutput(DigitalOutput&&)  noexcept = default;
  DigitalOutput& operator=(DigitalOutput&&)  noexcept;

  ResultCode configure(const Config::DigitalOutput::Fields& config);
  bool discover() override;
  ResultCode open() override;
  void close() override;
  void exit() override;

  // PttSink override
  // Note: This DigitalOutput may well have a setting path corresponding to PTT, but it doesn't
  // respond to ptt() since this is informational. It already responds to the applySetting*() so
  // to respond here as well would be circular.
  void ptt(bool on) override {};

  ResultCode applySettingUpdate(const SettingUpdate& setting) override;
  ResultCode applySettings(const RadioSettings& radioSettings) override { return ResultCode::OK; }

  void setValue(bool value);

protected:
  SettingPath m_settingPath;
  GpioOutputLinesSource m_linesSource;
  GpioOutputLinesRequest m_linesRequest;
};
