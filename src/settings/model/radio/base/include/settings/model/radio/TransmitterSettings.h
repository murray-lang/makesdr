#pragma once

#include "AfSettings.h"

class TransmitterSettings
{
public:
  using Proto = makesdr_TransmitterSettingsPb;

  TransmitterSettings(Proto& rawSettings)
    : m_rawSettings(rawSettings)
    ,  m_afSettings(rawSettings.mic)
  {}

  [[nodiscard]] bool hasMic() const { return m_rawSettings.has_mic; }
  [[nodiscard]] AfSettings& mic() { return m_afSettings; }
  [[nodiscard]] const AfSettings& mic() const { return m_afSettings; }

protected:
  Proto& m_rawSettings;
  AfSettings m_afSettings;
};