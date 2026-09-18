#pragma once
#include "DigitalOutputTypesT.h"
#include "settings/control/sink/SettingsControlSinkT.h"
#include "config/struct/DigitalOutputsConfig.h"
#include "DigitalOutputFactoryT.h"

template <typename RadioSettingsT>
class DigitalOutputsT : public SettingsControlSinkT<RadioSettingsT>, public FieldUpdateSink
{
public:
  DigitalOutputsT() = default;
  ~DigitalOutputsT() override = default;

  DigitalOutputsT(DigitalOutputsT&&)  noexcept = default;
  DigitalOutputsT& operator=(DigitalOutputsT&&)  noexcept = default;

  // ControlBase overrides;
  ResultCode configure(const Config::DigitalOutputs::Fields& config, ResolveDottedStringFunc resolver)
  {
    return createOutputs(config, resolver);
  }

  bool discover() override { return Gpio::isPresent(); }
  ResultCode open() override
  {
    for (auto& output : m_outputs) {
      ResultCode rc = ResultCode::OK;
      visit([&rc](auto&& dov)
      {
        rc = dov.open();
      }, output);
      if (rc != ResultCode::OK) {
        return rc;
      }
    }
    return ResultCode::OK;
  }

  void close() override
  {
    for (auto& output : m_outputs) {
      visit([](auto&& dov)
      {
        dov.close();
      }, output);
    }
  }

  void exit() override {}

  ResultCode applySettings(RadioSettingsT& settings) override
  {
    for (auto& output : m_outputs) {
      const ResultCode rc = visit([&settings] (auto&& dov) -> ResultCode
      {
        return dov.applySettings(settings);
      }, output);
      if (rc != ResultCode::OK) {
        return rc;
      }
    }
    return ResultCode::OK;
  }

  ResultCode applyFieldUpdate(const FieldUpdate& settingDelta) override
  {
    for (auto& output : m_outputs) {
      const ResultCode rc = visit([&settingDelta] (auto&& dov) -> ResultCode
      {
        return dov.applyFieldUpdate(settingDelta);
      }, output);
      if (rc != ResultCode::OK) {
        return rc;
      }
    }
    return ResultCode::OK;
  }

  ResultCode ptt(bool on) override
  {
    for (auto& output : m_outputs)
    {
      visit([on](auto&& dov)-> void
      {
        dov.ptt(on);
      }, output) ;
    }
    return ResultCode::OK;
  }

protected:
  ResultCode createOutputs(const Config::DigitalOutputs::Fields& config, ResolveDottedStringFunc resolver)
  {
    m_outputs.clear();
    ResultCode rc = ResultCode::OK;
    for (const auto& outputConfig : config.outputs) {
      typename DigitalOutputTypesT<RadioSettingsT>::Variant digitalOutput;
      rc = DigitalOutputFactoryT<RadioSettingsT>::create(outputConfig, resolver, digitalOutput);
      if (rc == ResultCode::OK) {
        m_outputs.emplace_back(std::move(digitalOutput));
      }
      return rc;
    }
    return rc;
  }

  DigitalOutputTypesT<RadioSettingsT>::VariantVector m_outputs;
};
