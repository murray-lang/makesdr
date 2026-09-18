#pragma once

#include <config/struct/ControlConfig.h>
#include "SettingsControlSinkTypesT.h"

template <typename RadioSettingsT>
class SettingsControlSinkFactoryT
{
public:
  static ResultCode create(
    const Config::Control::SinkConfigVariant& config,
    ResolveDottedStringFunc resolver,
    SettingsControlSinkTypesT<RadioSettingsT>::Variant& sink)
  {
    ResultCode result = ResultCode::OK;
    if (holds_alternative<Config::UsbControlSinks::Fields>(config)) {
      sink.template emplace<UsbControlSinksT<RadioSettingsT>>();
      // FunCubeDongle funCube;
      result = get<UsbControlSinksT<RadioSettingsT>>(sink).configure(get<Config::UsbControlSinks::Fields>(config));
      // if (result == ResultCode::OK) {
      //   sink.emplace<FunCubeDongle>(move(funCube));
      // }
      return result;
    }
#ifdef USE_GPIO
    if (holds_alternative<Config::DigitalOutputs::Fields>(config)) {
      DigitalOutputsT<RadioSettingsT> douts;
      result = douts.configure(get<Config::DigitalOutputs::Fields>(config), resolver);
      if (result == ResultCode::OK) {
        sink.template emplace<DigitalOutputsT<RadioSettingsT>>(move(douts));
      }
      return result;
    }
    // if (holds_alternative<Config::BandSelector::Fields>(config)) {
    //   GpioBandSelector bandSelector;
    //   result = bandSelector.configure(get<Config::BandSelector::Fields>(config));
    //   if (result == ResultCode::OK) {
    //     sink.emplace<GpioBandSelector>(move(bandSelector));
    //   }
    //   return result;
    // }
#endif

#ifdef IS_LINUX
    if (holds_alternative<Config::QtTransportOut::Fields>(config)) {
      using TransportSink = typename SettingsControlSinkTypesT<RadioSettingsT>::TransportSinkType;
      sink.template emplace<TransportSink>();
      result = get<TransportSink>(sink).configure(get<Config::QtTransportOut::Fields>(config));
      return result;
    }
#endif
    return ResultCode::ERR_SETTING_CONTROL_SINK_UNKNOWN_TYPE;
  }
};