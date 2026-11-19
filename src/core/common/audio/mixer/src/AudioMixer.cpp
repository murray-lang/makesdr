#include "audio/mixer/AudioMixer.h"
#include "audio/mixer/AudioMixerInput.h"

#include <algorithm>

AudioMixerBase::AudioMixerBase(
    AudioSink& output,
    RealSamplesRef& pendingA,
    RealSamplesRef& pendingB,
    RealSamplesRef& stereoOut
    )
  : m_inputA(*this, AudioMixerInput::INPUT_A)
  , m_inputB(*this, AudioMixerInput::INPUT_B)
  , m_output(output)
  , m_inputBEnabled(false)
  , m_havePendingA(false)
  , m_havePendingB(false)
  , m_pendingA(pendingA)
  , m_pendingB(pendingB)
  , m_stereoOutInterleaved(stereoOut)
  , m_gain(0.5f) // TODO: Feed configuration through to this. Turned down from 1.0 to prevent clipping.
{
}

uint32_t
AudioMixerBase::sinkAudioInput(
    AudioMixerInput::InputId inputId,
    const RealSamplesRef& samples,
    uint32_t length,
    uint32_t numChannels
    )
{
  const bool dual = m_inputBEnabled;

  if (!dual) {
    if (inputId != AudioMixerInput::INPUT_A) {
      return 0; // B arriving after being disabled: drop it.
    }
    // Flush anything left over from dual mode, in arrival order, then pass A straight through.
    if (m_havePendingA) {
      outputStereoFromSingle(m_pendingA);
      m_havePendingA = false;
    }
    m_havePendingB = false;
    const uint32_t frames = downmixToMono(samples, length, numChannels, m_pendingA);
    outputStereoFromSingle(m_pendingA);
    return frames;
  }

  const bool isA = (inputId == AudioMixerInput::INPUT_A);
  RealSamplesRef& mine = isA ? m_pendingA : m_pendingB;
  bool& haveMine = isA ? m_havePendingA : m_havePendingB;
  bool& haveOther = isA ? m_havePendingB : m_havePendingA;

  if (haveMine) {
    // This input delivered twice without the other: the other may have stalled.
    // Flush the older packet to both channels rather than dropping it or blocking.
    outputStereoFromSingle(mine);
    haveMine = false;
  }

  const uint32_t frames = downmixToMono(samples, length, numChannels, mine);
  if (frames == 0) {
    return 0;
  }

  if (haveOther) {
    outputStereoFromPair(m_pendingA, m_pendingB);
    m_havePendingA = false;
    m_havePendingB = false;
  } else {
    haveMine = true;
  }
  return frames;
}

uint32_t
AudioMixerBase::downmixToMono(const RealSamplesRef& in,
                              uint32_t length,
                              uint32_t numChannels,
                              RealSamplesRef& outMono) const
{
  if (length == 0 || numChannels == 0) {
    outMono.clear();
    return 0;
  }

  // Truncate to what both the input buffer and the stereo output can hold.
  const uint32_t frames = std::min<uint32_t>({
    length / numChannels,
    static_cast<uint32_t>(outMono.capacity()),
    static_cast<uint32_t>(m_stereoOutInterleaved.capacity() / 2)
  });
  outMono.resize(frames);

  if (numChannels == 1) {
    for (uint32_t f = 0; f < frames; ++f) {
      outMono[f] = in[f] * m_gain;
    }
    return frames;
  }

  const sdrreal scale = m_gain / static_cast<sdrreal>(numChannels);
  for (uint32_t f = 0; f < frames; ++f) {
    sdrreal acc = 0;
    const uint32_t base = f * numChannels;
    for (uint32_t c = 0; c < numChannels; ++c) {
      acc += in[base + c];
    }
    outMono[f] = acc * scale;
  }
  return frames;
}

void
AudioMixerBase::outputStereoFromPair(const RealSamplesRef& leftMono,
                                     const RealSamplesRef& rightMono)
{
  const uint32_t frames = static_cast<uint32_t>(std::min(leftMono.size(), rightMono.size()));
  if (frames == 0) {
    return;
  }

  m_stereoOutInterleaved.resize(frames * 2);
  for (uint32_t i = 0; i < frames; ++i) {
    m_stereoOutInterleaved[i * 2 + 0] = leftMono[i];
    m_stereoOutInterleaved[i * 2 + 1] = rightMono[i];
  }

  m_output.sinkAudio(m_stereoOutInterleaved,
                     static_cast<uint32_t>(m_stereoOutInterleaved.size()),
                     2);
}

void
AudioMixerBase::outputStereoFromSingle(const RealSamplesRef& mono)
{
  const uint32_t frames = static_cast<uint32_t>(mono.size());
  if (frames == 0) {
    return;
  }

  m_stereoOutInterleaved.resize(frames * 2);
  for (uint32_t i = 0; i < frames; ++i) {
    const sdrreal s = mono[i];
    m_stereoOutInterleaved[i * 2 + 0] = s;
    m_stereoOutInterleaved[i * 2 + 1] = s;
  }

  m_output.sinkAudio(m_stereoOutInterleaved,
                     static_cast<uint32_t>(m_stereoOutInterleaved.size()),
                     2);
}
