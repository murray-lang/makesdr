#include <CrossPlatformTypes.h>
#include "gpio/DigitalOutputs.h"
#include "gpio/DigitalOutputFactory.h"


ResultCode
DigitalOutputs::configure(const Config::DigitalOutputs::Fields& config)
{
  return createOutputs(config);
}

bool
DigitalOutputs::discover()
{
  return Gpio::isPresent();
}

ResultCode
DigitalOutputs::open()
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

void
DigitalOutputs::close()
{
  for (auto& output : m_outputs) {
    visit([](auto&& dov)
    {
      dov.close();
    }, output);
  }
}

void
DigitalOutputs::exit()
{

}

ResultCode
DigitalOutputs::createOutputs(const Config::DigitalOutputs::Fields& config)
{
  m_outputs.clear();
  ResultCode rc = ResultCode::OK;
  for (const auto& outputConfig : config.outputs) {
    DigitalOutputVariant digitalOutput;
    rc = DigitalOutputFactory::create(outputConfig, digitalOutput);
    if (rc == ResultCode::OK) {
      m_outputs.emplace_back(std::move(digitalOutput));
    }
    return rc;
  }
  return rc;
}

ResultCode
DigitalOutputs::applySettings(const BasicRadioSettings& settings)
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

ResultCode
DigitalOutputs::applySettingFieldUpdate(const SettingFieldUpdate& settingDelta)
{
  for (auto& output : m_outputs) {
    const ResultCode rc = visit([&settingDelta] (auto&& dov) -> ResultCode
    {
      return dov.applySettingFieldUpdate(settingDelta);
    }, output);
    if (rc != ResultCode::OK) {
      return rc;
    }
  }
  return ResultCode::OK;
}

void
DigitalOutputs::ptt(bool on)
{
  for (auto& output : m_outputs)
  {
    visit([on](auto&& dov)-> void
    {
      dov.ptt(on);
    }, output) ;
  }
}