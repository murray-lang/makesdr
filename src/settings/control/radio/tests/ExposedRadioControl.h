#pragma once

#include "RadioControl.h"

class ExposedRadioControl : public RadioControl
{
public:
  ExposedRadioControl();

  ControlSinkVector& getSinks() { return m_controlSinks;}
  ControlSourceVector& getSources() { return m_controlSources;}

  InternalSink& getInternalSink() { return m_internalSink;}
};
