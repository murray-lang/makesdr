#pragma once
#include "ControlConfig.h"
#include "UiConfig.h"
#include "ReceiverConfig.h"
#include "TransmitterConfig.h"

namespace Config::Radio
{
  static constexpr auto tag = "radio";

  struct Fields
  {
    Control::Fields control;
    optional<Receiver::Fields> receiver;
    optional<Transmitter::Fields> transmitter;
    optional<Ui::Fields> ui;
  };
}
