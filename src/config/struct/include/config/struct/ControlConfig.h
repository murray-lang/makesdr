#pragma once

#include <CrossPlatformTypes.h>
#include "BandSelectorConfig.h"
#include "FunCubeConfig.h"
#include "DigitalInputsConfig.h"
#include "DigitalOutputsConfig.h"
#include "QtTransportOutConfig.h"
#include "QtTransportInConfig.h"
#include "UsbControlSinksConfig.h"

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
  #ifdef IS_LINUX
    using SinkConfigVariant = variant<
      DigitalOutputs::Fields,
      UsbControlSinks::Fields,
      GpioLines::Fields,
      QtTransportOut::Fields
    >;

    using SourceConfigVariant = variant<DigitalInputs::Fields, QtTransportIn::Fields>;
  #else
    using SinkConfigVariant = variant<
      DigitalOutputs::Fields,
      UsbControlSinks::Fields,
      GpioLines::Fields
    >;
    using SourceConfigVariant = variant<DigitalInputs::Fields>;
  #endif // IS_LINUX
#else
  #ifdef IS_LINUX
    using SinkConfigVariant = variant<
      UsbControlSinks::Fields,
      QtTransportOut::Fields
    >;
    using SourceConfigVariant = variant<QtTransportIn::Fields>;
  #else
    using SinkConfigVariant = variant<
      UsbControlSinks::Fields
    >;
    using SourceConfigVariant = variant<monostate>;
  #endif // IS_LINUX
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
