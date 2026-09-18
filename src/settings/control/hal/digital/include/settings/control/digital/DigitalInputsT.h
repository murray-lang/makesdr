#pragma once

#include <settings/control/source/SettingsControlSourceT.h>
#include <config/struct/DigitalInputsConfig.h>
#include <settings/model/radios/IRadioSettings.h>
// #include <stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h>

#include <settings/control/digital/DigitalInputTypes.h>
#include "DigitalInputFactory.h"

template <typename RadioSettingsT>
class DigitalInputsT : public SettingsControlSourceT<RadioSettingsT>
{
public:

  explicit DigitalInputsT(const char* consumer = "")
    : m_internalSink(*this)
  {
  }
  ~DigitalInputsT() override = default;

  DigitalInputsT(DigitalInputsT&& rhs)  noexcept
  : SettingsControlSourceT<RadioSettingsT>(::move(rhs))
  , m_internalSink(*this) // Reference to new object
  , m_inputs(::move(rhs.m_inputs))
  // m_linesRequest(move(rhs.m_linesRequest)),
  // m_lineToInputMap(move(rhs.m_lineToInputMap))
{
    reconnectInputSinks();
}
  DigitalInputsT& operator=(DigitalInputsT&& rhs)  noexcept
  {
    if (this != &rhs) {
      SettingsControlSourceT<RadioSettingsT>::operator=(move(rhs));
      m_inputs = ::move(rhs.m_inputs);
      // m_linesRequest = move(rhs.m_linesRequest);
      // m_lineToInputMap = move(rhs.m_lineToInputMap);
      reconnectInputSinks();
    }
    return *this;
  }

  // ControlBase overrides;
  ResultCode configure(const Config::DigitalInputs::Fields& config, ResolveDottedStringFunc resolver)
  {
    return createInputs(config, resolver);
  }
  bool discover() override { return Gpio::isPresent(); }
  ResultCode open() override
  {
    // return static_cast<ResultCode>(m_inputs.at(1).getLines());
    GpioInputLinesRequestVector inputs;
    for (auto& input : m_inputs) {
      inputs.emplace_back(&input.getLinesRequest());
    }
    // if (!inputs.at(1)->config.isRotaryEncoder()) {
    //   return static_cast<ResultCode>(1234);
    // }
    // return static_cast<ResultCode>(inputs.at(1)->config.getLines());
    Gpio& gpio = Gpio::getInstance();
    ResultCode rc = gpio.requestInputs("digitalInputs", inputs);
    if (rc != ResultCode::OK) {
      return rc;
    }
    return gpio.startInputs();
  }
  void close() override {}
  void exit() override {}

  // [[nodiscard]] const ThreadRequirements* getThreadRequirements() const override;
  // void run() override;
  // void quit() override;
  // bool tick() override;

  [[nodiscard]] const DigitalInputVector& getInputs() const { return m_inputs; }

protected:
  ResultCode notifySettings(RadioSettingsT& radioSettings) override
  {
    return ResultCode::ERR_SETTING_CONTROL_NOTIFY_SETTINGS_NOT_IMPLEMENTED;
  }

  ResultCode createInputs(const Config::DigitalInputs::Fields& config, ResolveDottedStringFunc resolver)
  {
    m_inputs.clear();
    ResultCode rc = ResultCode::OK;
    for (const auto& inputConfig : config.inputs) {
      DigitalInput digitalInput;
      rc = DigitalInputFactory::create(inputConfig, digitalInput, resolver);
      if (rc == ResultCode::OK) {
        digitalInput.connectFieldUpdateSink(&m_internalSink);
        m_inputs.emplace_back(::move(digitalInput));
      } else {
        break;
      }
    }
    return rc;
  }

  void reconnectInputSinks()
  {
    for (auto& input : m_inputs) {
      input.connectFieldUpdateSink(&m_internalSink);
    }
  }

  class InternalSink : public FieldUpdateSink
  {
  public:
    explicit InternalSink(DigitalInputsT& group) : m_group(group) {}

    ResultCode applyFieldUpdate(const FieldUpdate& settingDelta) override
    {
      return m_group.get().notifyFieldUpdate(settingDelta);
    }

  protected:
    reference_wrapper<DigitalInputsT> m_group;
  };

protected:
  InternalSink m_internalSink;
  DigitalInputVector m_inputs;
  // optional<DigitalInputLinesRequest> m_linesRequest;
  // DigitalInputMap m_lineToInputMap;
};
