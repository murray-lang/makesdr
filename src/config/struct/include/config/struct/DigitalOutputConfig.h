#pragma once

#include "ConfigBase.h"
#include "GpioLinesConfig.h"
#include "FieldDescriptorConfig.h"

#ifdef USE_ETL
#include <etl/string.h>
#else
#include <string>
#endif

namespace Config::DigitalOutput
{
  static constexpr auto type = "digitaloutput";

  struct Fields : GpioLines::Fields, Alternative
  {
    optional<FieldPathString> settingPath;
    optional<Config::FieldDescriptor::Fields> settingDescriptor;
  };
}

// using DigitalOutputConfig = Config::DigitalOutput::Fields;
