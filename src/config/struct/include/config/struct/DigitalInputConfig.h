#pragma once

#include "ConfigBase.h"
#include "GpioInputLinesConfig.h"
#include "FieldDescriptorConfig.h"



namespace Config::DigitalInput
{
  static constexpr auto type = "digitalinput";

  struct Fields : GpioInputLines::Fields, Alternative
  {
    optional<FieldPathString> settingPath;
    optional<FieldDescriptor::Fields> settingDescriptor;
  };
}

// using DigitalInputConfig = Config::DigitalInput::Fields;
