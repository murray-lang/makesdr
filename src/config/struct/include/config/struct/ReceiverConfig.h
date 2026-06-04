#pragma once

#include "IqIoConfig.h"

namespace Config::Receiver
{
  static constexpr auto type = "receiver";

  struct Fields
  {
    IqIo::Fields iqIo;
  };
}

// using ReceiverConfig = Config::Receiver::Fields;
