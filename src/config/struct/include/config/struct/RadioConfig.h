#pragma once
#include "CapabilitiesConfig.h"
#include "ControlConfig.h"
#include "UiConfig.h"
#include "SdrConfig.h"

namespace Config::Radio
{
  static constexpr auto tag = "radio";

  struct Fields
  {
    Capabilities::Fields capabilities;
    Control::Fields control;
    optional<Sdr::Fields> sdr;
    optional<Ui::Fields> ui;
  };
}
