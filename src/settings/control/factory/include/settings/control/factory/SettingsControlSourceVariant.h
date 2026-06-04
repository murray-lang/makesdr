#pragma once
#include <config/struct/ControlConfig.h>

#ifdef USE_ETL
#include <etl/vector.h>
#include <etl/variant.h>
#include <etl/optional.h>
#include <etl/string.h>
#include <etl/memory.h>
#else
#include <vector>
#include <variant>
#include <optional>
#include <string>
#include <memory>
#endif



#ifdef USE_ETL
using etl::variant;
using etl::optional;
using etl::unique_ptr;
#else
using std::variant;
using std::optional;
using std::unique_ptr;
#endif

#include <settings/control/source/SettingsControlSource.h>
#ifdef USE_GPIO
#include <settings/control/gpio/DigitalInputs.h>
#endif

#ifdef IS_QT
#include <settings/control/qt/QtSettingsControlSource.h>
#endif
// #include "core/config-settings/config/base/ConfigBase.h"

#ifdef USE_GPIO
#ifdef IS_QT
using SettingsControlSourceVariant = variant<QtSettingsControlSource, DigitalInputs>;
#else
using SettingsControlSourceVariant = variant<DigitalInputs>;
#endif // IS_QT
#else
#ifdef IS_QT
using SettingsControlSourceVariant = variant<QtSettingsControlSource>;
#else
using SettingsControlSourceVariant = variant<monostate>;
#endif
#endif