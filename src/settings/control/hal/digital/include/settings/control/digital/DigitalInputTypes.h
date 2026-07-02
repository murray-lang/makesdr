#pragma once

#include "settings/control/digital/DigitalInput.h"

#ifdef USE_ETL
#include "etl/vector.h"


// This is declared as a class to facilitate forward declarations
class DigitalInputVector : public etl::vector<DigitalInput, MAX_DIGITAL_INPUT_HANDLERS> {
public:
  using etl::vector<DigitalInput, MAX_DIGITAL_INPUT_HANDLERS>::vector;
};
#else
#include <variant>

class DigitalInputVector : public std::vector<DigitalInput> {
public:
  using std::vector<DigitalInput>::vector;
};

#endif