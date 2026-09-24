#pragma once

#include "SampleTypes.h"
#include <cstdint>

template<typename SamplesType> class PingPongBuffers
{
public:
  // Size to capacity up front. Pipeline stages index these with at() without
  // resizing first, so size() has to cover the whole buffer or every access is
  // out of range once ETL checks are live.
  explicit PingPongBuffers() : m_flip(false)
  {
    m_ping.resize(m_ping.max_size());
    m_pong.resize(m_pong.max_size());
  }

  SamplesType& input()
  {
    return m_flip ? m_pong : m_ping;
  }
  SamplesType& output() { return m_flip ? m_ping : m_pong; }

  SamplesType& current() { return m_flip ? m_pong : m_ping; }
  SamplesType& next() { return m_flip ? m_ping : m_pong; }

  PingPongBuffers& flip()
  {
    m_flip = !m_flip;
    return *this;
  }
  PingPongBuffers& reset() { m_flip = false; return *this; }

  PingPongBuffers& resize(uint32_t length) {
    m_ping.resize(length);
    m_pong.resize(length);
    return *this;
  }

  [[nodiscard]] uint32_t getSize() const { return m_ping.max_size(); }

private:
  SamplesType m_ping;
  SamplesType m_pong;
  bool m_flip;
};

using ComplexPingPongBuffers = PingPongBuffers<ComplexSamplesBuffer>;
using RealPingPongBuffers = PingPongBuffers<RealSamplesBuffer>;
