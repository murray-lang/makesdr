#include <ResultCode.h>
#include "../include/settings/control/factory/SettingsControlSinkFactory.h"

// #ifdef USE_GPIO
// #include "config/struct/BandSelectorConfig.h"
// #include "settings/control/gpio/GpioBandSelector.h"
// #include "settings/control/gpio/digital/DigitalOutput.h"
// #include "settings/control/gpio/digital/DigitalOutputs.h"
// #endif

//#ifdef IS_QT
//#include "settings/control//QtSettingsControlSink.h"
//#endif

#include "config/struct/FunCubeConfig.h"
#include "config/struct/DigitalOutputsConfig.h"
#include "config/struct/QtControlSinkConfig.h"


ResultCode
SettingsControlSinkFactory::create(const Config::Control::SinkConfigVariant& config, SettingsControlSinkVariant& sink)
{
  ResultCode result = ResultCode::OK;
  if (holds_alternative<Config::FunCube::Fields>(config)) {
    FunCubeDongle funCube;
    result = funCube.configure(get<Config::FunCube::Fields>(config));
    if (result == ResultCode::OK) {
      sink.emplace<FunCubeDongle>(funCube);
    }
    return result;
  }
#ifdef USE_GPIO
  if (holds_alternative<Config::DigitalOutputs::Fields>(config)) {
    DigitalOutputs douts;
    result = douts.configure(get<Config::DigitalOutputs::Fields>(config));
    if (result == ResultCode::OK) {
      sink.emplace<DigitalOutputs>(move(douts));
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

#ifdef IS_QT
  if (holds_alternative<Config::QtControlSink::Fields>(config)) {
    QtSettingsControlSink qtcs;
    result = qtcs.configure(get<Config::QtControlSink::Fields>(config));
    if (result == ResultCode::OK) {
      sink.emplace<QtSettingsControlSink>(move(qtcs));
    }
    return result;
  }
#endif
  return ResultCode::ERR_SETTING_CONTROL_SINK_UNKNOWN_TYPE;
}
