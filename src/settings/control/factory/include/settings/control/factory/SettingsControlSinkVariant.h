#pragma once

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

#ifdef USE_GPIO
#include "settings/control/gpio/DigitalOutputs.h"
#include "settings/control/gpio/DigitalOutput.h"
#include "settings/control/gpio/GpioBandSelector.h"
#endif

#ifdef IS_QT
#include "settings/control/qt/QtSettingsControlSink.h"
#endif

#include "settings/control/FunCubeDongle/FunCubeDongle.h"


#ifdef USE_GPIO
  #ifdef IS_QT
    using SettingsControlSinkVariant = variant<FunCubeDongle, DigitalOutputs, QtSettingsControlSink>;
  #else
    using SettingsControlSinkVariant = variant<FunCubeDongle, DigitalOutputs>;
  #endif
#else
  #ifdef IS_QT
    using SettingsControlSinkVariant = variant<FunCubeDongle, QtSettingsControlSink>;
  #else
    using SettingsControlSinkVariant = variant<FunCubeDongle>;
  #endif
#endif
