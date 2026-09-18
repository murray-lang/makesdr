#pragma once

#include <ResultCode.h>
#include <config/struct/ControlConfig.h>
#include "SettingsControlSourceTypesT.h"

#ifdef USE_GPIO
#include <config/struct/DigitalInputsConfig.h>
#include <settings/control/digital/DigitalInputsT.h>
#endif

#ifdef IS_LINUX
#include <config/struct/QtTransportInConfig.h>
#include <settings/control/transport/TransportControlSourceT.h>
#endif

template <typename RadioSettingsT>
class SettingsControlSourceFactoryT
{
public:
  static ResultCode create(
    const Config::Control::SourceConfigVariant& config,
    ResolveDottedStringFunc resolver,
    SettingsControlSourceTypesT<RadioSettingsT>::Variant& source)
  {
    ResultCode result = ResultCode::OK;
#ifdef USE_GPIO
    if (holds_alternative<Config::DigitalInputs::Fields>(config)) {
      DigitalInputsT<RadioSettingsT> dins;
      result = dins.configure(get<Config::DigitalInputs::Fields>(config), resolver);
      if (result == ResultCode::OK) {
        source.template emplace<DigitalInputsT<RadioSettingsT>>(::move(dins));
      }
      return result;
    }
#endif
#ifdef IS_LINUX
    if (holds_alternative<Config::QtTransportIn::Fields>(config)) {
      TransportControlSourceT<RadioSettingsT> qtcs;
      result = qtcs.configure(get<Config::QtTransportIn::Fields>(config));
      if (result == ResultCode::OK) {
        source.template emplace<TransportControlSourceT<RadioSettingsT>>(::move(qtcs));
      }
      return result;
    }
#endif
    return ResultCode::ERR_SETTING_CONTROL_NO_SOURCES_DEFINED;
  }
};