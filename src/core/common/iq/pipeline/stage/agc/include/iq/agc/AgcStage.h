#pragma once

#include <iq/pipeline/stage/IqPipelineStage.h>
#include <liquid/liquid.h>

#include <algorithm>
#include <stdexcept>
#include <complex>

#include <settings/model/radio/AgcSpeed.h>

constexpr float SLOW_LOOP_BANDWIDTH = 1e-4f;
constexpr float MEDIUM_LOOP_BANDWIDTH = 1e-3f;
constexpr float FAST_LOOP_BANDWIDTH = 1e-2f;
constexpr float DEFAULT_LOOP_BANDWIDTH = MEDIUM_LOOP_BANDWIDTH;

class AgcStage : public IqPipelineStage
{
public:
  AgcStage(AgcSpeed speed = AgcSpeed::DEFAULT);

  ~AgcStage() override;

  void setSpeed(AgcSpeed speed);
  [[nodiscard]] AgcSpeed getSpeed() const { return m_speed; }
  [[nodiscard]] float getBandwidth() const { return agc_crcf_get_bandwidth(m_agc); }

  bool isOff() const { return m_speed == AgcSpeed::OFF; }

  uint32_t processSamples(ComplexPingPongBuffers& buffers, uint32_t inputLength) override;

  [[nodiscard]] float getGainDb() const;

protected:
  [[nodiscard]] float speedToBandwidth(AgcSpeed speed) const;
  void setBandwidth(AgcSpeed speed);

  void setBandwidth(float bandwidth);

private:
  agc_crcf m_agc;
  std::atomic<float> m_lastGain;
  AgcSpeed m_speed;
};