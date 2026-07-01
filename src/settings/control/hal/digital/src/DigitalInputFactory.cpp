#include "settings/control/digital/DigitalInputFactory.h"


ResultCode
DigitalInputFactory::create(const Config::DigitalInput::Fields& config, DigitalInput& input)
{
  return input.configure(config);
}
