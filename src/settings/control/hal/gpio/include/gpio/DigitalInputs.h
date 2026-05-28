#pragma once

#include <control/source/SettingsControlSource.h>
#include "DigitalInputLinesRequest.h"
#include "config/DigitalInputsConfig.h"
#include "DigitalInputTypes.h"
#include "../../../../../model/basic/include/settings/basic/BasicRadioSettings.h"


class DigitalInputs : public SettingsControlSource, public DigitalInputLinesRequest::Callback
{
public:

  explicit DigitalInputs(const char* consumer = "");
  ~DigitalInputs() override;

  DigitalInputs(DigitalInputs&& rhs)  noexcept;
  DigitalInputs& operator=(DigitalInputs&& rhs)  noexcept;

  // ControlBase overrides;
  ResultCode configure(const Config::DigitalInputs::Fields& config);
  bool discover() override;
  ResultCode open() override;
  void close() override;
  void exit() override;

  // GpioLines::Callback override
  void callback(DigitalInputLinesRequest::LineStateVector& lineStates) override;

protected:
  ResultCode notifySettings(const BasicRadioSettings& radioSettings) override
  {
    return ResultCode::ERR_SETTING_CONTROL_NOTIFY_SETTINGS_NOT_IMPLEMENTED;
  }

  ResultCode createInputs(const Config::DigitalInputs::Fields& config);

  void createLineToInputMap();

  void readInitialInputStates();

  void reconnectInputSinks();

  class InternalSink : public SettingFieldUpdateSink
  {
  public:
    explicit InternalSink(DigitalInputs& group) : m_group(group) {}

    ResultCode applySettingFieldUpdate(const SettingFieldUpdate& settingDelta) override
    {
      return m_group.get().notifySettingFieldUpdate(settingDelta);
    }

  protected:
    reference_wrapper<DigitalInputs> m_group;
  };

protected:
  InternalSink m_internalSink;
  DigitalInputVariantVector m_inputs;
  optional<DigitalInputLinesRequest> m_linesRequest;
  DigitalInputMap m_lineToInputMap;
};
