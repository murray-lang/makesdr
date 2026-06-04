#pragma once

#include <CrossPlatformTypes.h>
#include "BandSelectorConfig.h"
#include "FunCubeConfig.h"
#include "DigitalInputsConfig.h"
#include "DigitalOutputsConfig.h"
#include "QtControlSinkConfig.h"
#include "QtControlSourceConfig.h"

#ifdef USE_ETL
using etl::variant;
using etl::optional;
#else
using std::variant;
using std::optional;
#endif

namespace Config::Control
{
  static constexpr auto type = "control";
#ifdef USE_GPIO
  #ifdef IS_QT
    using SinkConfigVariant = variant<
      DigitalOutputs::Fields,
      FunCube::Fields,
      GpioLines::Fields,
      QtControlSink::Fields
    >;

    using SourceConfigVariant = variant<DigitalInputs::Fields, QtControlSource::Fields>;
  #else
    using SinkConfigVariant = variant<
      DigitalOutputs::Fields,
      FunCube::Fields,
      GpioLines::Fields
    >;
    using SourceConfigVariant = variant<DigitalInputs::Fields>;
  #endif // IS_QT
#else
  #ifdef IS_QT
    using SinkConfigVariant = variant<
      FunCube::Fields,
      QtControlSink::Fields
    >;
    using SourceConfigVariant = variant<QtControlSource::Fields>;
  #else
    using SinkConfigVariant = variant<
      FunCube::Fields
    >;
    using SourceConfigVariant = variant<monostate>;
  #endif // IS_QT
#endif


#ifdef USE_ETL
  using SinkVector = etl::vector<SinkConfigVariant, MAX_CONTROL_SINKS>;
  using SourceVector = etl::vector<SourceConfigVariant, MAX_CONTROL_SOURCES>;
#else
  // using SinkVariant = SinkVariantT<std::variant>;
  // using SourceVariant = SourceVariantT<std::variant>;
  // using OptionalSinkVariant = std::optional<SinkVariant>;
  // using OptionalSourceVariant = std::optional<SourceVariant>;
  using SinkVector = std::vector<SinkConfigVariant>;
  using SourceVector = std::vector<SourceConfigVariant>;
#endif

  struct Fields
  {
    SinkVector sinks;
    SourceVector sources;
  };
}

// using ControlConfig = Config::Control::Fields;
