#pragma once

#include "ConfigBase.h"

#ifdef USE_ETL
#include <etl/string.h>
#else
#include <string>
#endif

// #define TARGET_STRING_LENGTH_INCL_0 8

namespace Config::QtTransport
{
  enum Message { None, Settings, Update, Modes, Bands, Meter, Iq };

  enum Target { Radio, Client };
#ifdef USE_ETL
  using TargetString = etl::string<TARGET_STRING_LENGTH_INCL_0>;
#else
  using TargetString = std::string;
#endif

  struct Fields : Alternative
  {
    Message message{Settings};
    Target target{Radio};
  };
}