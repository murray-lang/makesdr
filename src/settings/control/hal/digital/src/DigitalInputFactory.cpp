#include "settings/control/digital/DigitalInputFactory.h"


ResultCode
DigitalInputFactory::create(
  const Config::DigitalInput::Fields& config,
  DigitalInput& input,
  ResolveDottedStringFunc resolver)
{
  return input.configure(config, resolver);
}
