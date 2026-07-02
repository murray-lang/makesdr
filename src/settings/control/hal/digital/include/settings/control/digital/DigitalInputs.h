#pragma once

#include <settings/control/source/SettingsControlSource.h>
#include <config/struct/DigitalInputsConfig.h>
#include <settings/model/core/RadioSettings.h>
#include <stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h>

#include "settings/control/digital//DigitalInputTypes.h"


class DigitalInputs : public SettingsControlSource
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

  [[nodiscard]] const DigitalInputVector& getInputs() const { return m_inputs; }

protected:
  ResultCode notifySettings(const RadioSettings& radioSettings) override
  {
    return ResultCode::ERR_SETTING_CONTROL_NOTIFY_SETTINGS_NOT_IMPLEMENTED;
  }

  ResultCode createInputs(const Config::DigitalInputs::Fields& config);

  void reconnectInputSinks();

  class InternalSink : public SettingUpdateSink
  {
  public:
    explicit InternalSink(DigitalInputs& group) : m_group(group) {}

    ResultCode applySettingUpdate(const SettingUpdate& settingDelta) override
    {
      return m_group.get().notifySettingUpdate(settingDelta);
    }

  protected:
    reference_wrapper<DigitalInputs> m_group;
  };

protected:
  InternalSink m_internalSink;
  DigitalInputVector m_inputs;
  // optional<DigitalInputLinesRequest> m_linesRequest;
  // DigitalInputMap m_lineToInputMap;
};
