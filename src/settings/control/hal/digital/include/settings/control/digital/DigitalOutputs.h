#pragma once
#include "DigitalOutputTypes.h"
#include "settings/control/sink/SettingsControlSink.h"
#include "config/struct/DigitalOutputsConfig.h"

class DigitalOutputs : public SettingsControlSink, public SettingUpdateSink
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

  ResultCode applySettings(const RadioSettings& settings) override;
  ResultCode applySettingUpdate(const SettingUpdate& settingDelta) override;

  void ptt(bool on) override;

protected:
  ResultCode createOutputs(const Config::DigitalOutputs::Fields& config);

  DigitalOutputVariantVector m_outputs;
};
