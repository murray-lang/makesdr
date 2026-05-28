#pragma once
#include "DigitalOutputTypes.h"
#include "control/sink/SettingsControlSink.h"
#include "config/DigitalOutputsConfig.h"

class DigitalOutputs : public SettingsControlSink, public SettingFieldUpdateSink
{
public:
  DigitalOutputs() = default;
  ~DigitalOutputs() override = default;

  DigitalOutputs(DigitalOutputs&&)  noexcept = default;
  DigitalOutputs& operator=(DigitalOutputs&&)  noexcept = default;

  // ControlBase overrides;
  ResultCode configure(const Config::DigitalOutputs::Fields& config);
  bool discover() override;
  ResultCode open() override;
  void close() override;
  void exit() override;

  ResultCode applySettings(const BasicRadioSettings& settings) override;
  ResultCode applySettingFieldUpdate(const SettingFieldUpdate& settingDelta) override;

  void ptt(bool on) override;

protected:
  ResultCode createOutputs(const Config::DigitalOutputs::Fields& config);

  DigitalOutputVariantVector m_outputs;
};
