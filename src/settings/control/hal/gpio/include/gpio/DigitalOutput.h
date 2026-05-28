#pragma once

#include <CrossPlatformTypes.h>
#include "config/DigitalOutputConfig.h"
#include "control/sink/SettingsControlSink.h"
#include "gpio/base/GpioLines.h"
#include "../../../../../model/basic/include/model/basic/BasicRadioSettingsSink.h"
#include "model/SettingFieldPath.h"
#include "gpio/DigitalOutputLinesRequest.h"
#include "model/SettingFieldUpdate.h"
#include "model/SettingFieldUpdateSink.h"

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
  ResultCode applySettings(const BasicRadioSettings& radioSettings) override { return ResultCode::OK; }

  void setValue(bool value);

protected:
  SettingFieldPath m_settingPath;
  optional<DigitalOutputLinesRequest> m_linesRequest;

};
