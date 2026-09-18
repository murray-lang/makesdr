#pragma once

#include <config/struct/QtTransportOutConfig.h>
#include <config/struct/QtTransportInConfig.h>

struct QtTransportIoConfig
{
  Config::QtTransportIn::Fields in;
  Config::QtTransportOut::Fields out;
};
