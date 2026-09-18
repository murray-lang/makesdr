#pragma once
#include <CrossPlatformTypes.h>
#include <config/struct/ControlConfig.h>

#ifdef USE_ETL
#include <etl/vector.h>
#else
#include <vector>
#endif

#ifdef USE_GPIO
#include <settings/control/digital/DigitalInputsT.h>
#endif

#ifdef IS_LINUX
#include <settings/control/transport/TransportControlSourceT.h>
#endif


template <typename RadioSettingsT>
struct SettingsControlSourceTypesT
{
#ifdef USE_GPIO
#ifdef IS_LINUX
  using Variant = variant<TransportControlSourceT<RadioSettingsT>, DigitalInputsT<RadioSettingsT>>;
#else
  using Variant = variant<DigitalInputsT<RadioSettingsT>>;
#endif // IS_LINUX
#else
#ifdef IS_QT
  using Variant = variant<QtSettingsControlSourceT<RadioSettingsT>>;
#else
  using Variant = variant<monostate>;
#endif
#endif
};