#include "../include/settings/control/factory/SettingsControlSourceFactory.h"

#ifdef USE_GPIO
#include <config/struct/DigitalInputsConfig.h>
#include <settings/control/gpio/DigitalInputs.h>
#include "settings/control/gpio/RotaryEncoder.h"
#endif

#ifdef IS_QT
#include <config/struct/QtControlSourceConfig.h>
#include <settings/control/qt/QtSettingsControlSource.h>
#endif



ResultCode
SettingsControlSourceFactory::create(
  const Config::Control::SourceConfigVariant& config,
  SettingsControlSourceVariant& source
  )
{
  ResultCode result = ResultCode::OK;
#ifdef USE_GPIO
  if (holds_alternative<Config::DigitalInputs::Fields>(config)) {
    DigitalInputs dins;
    result = dins.configure(get<Config::DigitalInputs::Fields>(config));
    if (result == ResultCode::OK) {
      source.emplace<DigitalInputs>(move(dins));
    }
    return result;
  }
#endif
#ifdef IS_QT
  if (holds_alternative<Config::QtControlSource::Fields>(config)) {
    QtSettingsControlSource qtcs;
    result = qtcs.configure(get<Config::QtControlSource::Fields>(config));
    if (result == ResultCode::OK) {
      source.emplace<QtSettingsControlSource>(move(qtcs));
    }
    return result;
  }
#endif
  return ResultCode::ERR_SETTINGS_CONTROL_NO_SOURCES_DEFINED;
}
