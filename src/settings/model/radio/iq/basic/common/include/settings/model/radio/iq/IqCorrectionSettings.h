#pragma once
#include <settings/model/proto/RadioSettings.pb.h>
//#include "SteppableFloatSetting.h"

class IqCorrectionSettings
{
public:
  using Proto = makesdr_IqCorrectionSettingsPb;

  IqCorrectionSettings(Proto& raw)
  : m_rawSettings(raw)
  // , m_amplitude(raw.amplitude)
  // , m_phase(raw.phase)
  {}

  [[nodiscard]] bool hasAmplitude() const { return m_rawSettings.has_amplitude; }
  [[nodiscard]] bool hasPhase() const { return m_rawSettings.has_phase; }

  [[nodiscard]] float amplitude() const { return m_rawSettings.amplitude.value; }
  [[nodiscard]] float phase() const { return m_rawSettings.phase.value; }
  //SteppableFloatSetting& amplitude() { return m_amplitude; }
  //SteppableFloatSetting& phase() { return m_phase; }
  //[[nodiscard]] const SteppableFloatSetting& amplitude() const { return m_amplitude; }
  //[[nodiscard]] const SteppableFloatSetting& phase() const { return m_phase; }

protected:
  Proto& m_rawSettings;
  //SteppableFloatSetting m_amplitude;
  //SteppableFloatSetting m_phase;
};
