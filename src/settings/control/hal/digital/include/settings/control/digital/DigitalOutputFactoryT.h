#pragma once

#include <CrossPlatformTypes.h>
#include "DigitalOutputTypesT.h"
#include "config/struct/DigitalOutputsConfig.h"

template <typename RadioSettingsT>
class DigitalOutputFactoryT
{
public:
  static ResultCode create(
    const Config::DigitalOutputs::DigitalOutputConfigVariant& config,
    ResolveDottedStringFunc resolver,
    DigitalOutputTypesT<RadioSettingsT>::Variant& output)
  {
    ResultCode result = ResultCode::OK;
    if (holds_alternative<Config::DigitalOutput::Fields>(config)) {
      DigitalOutputT<RadioSettingsT> dout;
      result = dout.configure(get<Config::DigitalOutput::Fields>(config), resolver);
      if (result == ResultCode::OK) {
        output.template emplace<DigitalOutputT<RadioSettingsT>>(::move(dout));
      }
      return result;
    }
    if (holds_alternative<Config::BandSelector::Fields>(config)) {
      GpioBandSelectorT<RadioSettingsT> selector;
      result = selector.configure(get<Config::BandSelector::Fields>(config), resolver);
      if (result == ResultCode::OK) {
        output.template emplace<GpioBandSelectorT<RadioSettingsT>>(::move(selector));
      }
      return result;
    }
    return ResultCode::ERR_SETTING_CONTROL_UNKNOWN_DIGITAL_OUTPUT_TYPE;
  }
};
