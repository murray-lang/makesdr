#pragma once

#include <CrossPlatformTypes.h>
#include "config/struct/DigitalOutputConfig.h"
#include "settings/control/sink/SettingsControlSink.h"
#include "settings/control/gpio/base/GpioLines.h"
#include "settings/model/core/RadioSettingsSink.h"
#include "settings/model/core/SettingFieldPath.h"
#include "settings/control/gpio/DigitalOutputLinesRequest.h"
#include "settings/model/core/SettingFieldUpdate.h"
#include "settings/model/core/SettingFieldUpdateSink.h"

#ifdef USE_ETL
#include "etl/memory.h"
using etl::unique_ptr;
#else
#include <memory>
using std::unique_ptr;
#endif


class DigitalOutput : public GpioLines, public SettingsControlSink, public SettingFieldUpdateSink
{
public:
  DigitalOutput();
  ~DigitalOutput() override = default;

  DigitalOutput(DigitalOutput&&)  noexcept = default;
  DigitalOutput& operator=(DigitalOutput&&)  noexcept = default;

  virtual ResultCode configure(const Config::DigitalOutput::Fields& config);
  bool discover() override;
  ResultCode open() override;
  void close() override;
  void exit() override;

  // PttSink override
  // Note: This DigitalOutput may well have a setting path corresponding to PTT, but it doesn't
  // respond to ptt() since this is informational. It already responds to the applySetting*() so
  // to respond here as well would be circular.
  void ptt(bool on) override {};

  ResultCode applySettingFieldUpdate(const SettingFieldUpdate& setting) override;
  ResultCode applySettings(const RadioSettings& radioSettings) override { return ResultCode::OK; }

  void setValue(bool value);

protected:
  SettingFieldPath m_settingPath;
  optional<DigitalOutputLinesRequest> m_linesRequest;

};
