#pragma once
#include <settings/model/proto/RadioSettings.pb.h>

class AfSettings
{
public:
	using Proto = makesdr_AfSettingsPb;

  AfSettings(Proto& rawSettings)
    : m_rawSettings(rawSettings)
  {}

  [[nodiscard]] bool hasGain() const { return m_rawSettings.has_gain; }
  [[nodiscard]] float gain() const { return m_rawSettings.gain.value; }

protected:
  Proto& m_rawSettings;
};
