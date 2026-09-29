#include "audio/mixer/AudioMixer.h"
#include "audio/mixer/AudioMixerInput.h"

AudioMixer::AudioMixer(AudioSink& output)
  : m_inputA(*this, AudioMixerInput::INPUT_A)
  , m_inputB(*this, AudioMixerInput::INPUT_B)
  , m_inputBEnabled(false)
  , m_output(output)
  , m_havePendingA(false)
  , m_havePendingB(false)
  , m_gain(1000.0f)
{
}

uint32_t
AudioMixer::sinkAudioInput(
    AudioMixerInput::InputId inputId,
    const RealSamplesBuffer& samples,
    uint32_t length,
    uint32_t numChannels
    )
{

  RealSamplesBuffer mono;
  const uint32_t frames = downmixToMono(samples, length, numChannels, mono);
  if (frames == 0) {
    return 0;
  }

  if (!m_inputBEnabled) {
    // A-only mode: duplicate to stereo. (If B accidentally calls, still harmless.)
    outputStereoDuplicate(mono, frames);
    return frames;
  }

  if (inputId == AudioMixerInput::INPUT_A) {
    m_pendingA.swap(mono);
    m_havePendingA = true;

    // If B already arrived first (unexpected), pair immediately.
    if (m_havePendingB) {
      const uint32_t n = std::min<uint32_t>(
        static_cast<uint32_t>(m_pendingA.size()),
        static_cast<uint32_t>(m_pendingB.size())
      );
      outputStereoFromMono(m_pendingA, m_pendingB, n);
      m_havePendingA = false;
      m_havePendingB = false;
    }
    return frames;
  }

  // InputId::B
  if (m_havePendingA) {
    const uint32_t n = std::min<uint32_t>(
      static_cast<uint32_t>(m_pendingA.size()),
      static_cast<uint32_t>(mono.size())
    );
    outputStereoFromMono(m_pendingA, mono, n);
    m_havePendingA = false;
    return frames;
  }

  // B arrived before A (should be rare). Keep it for pairing.
  m_pendingB.swap(mono);
  m_havePendingB = true;
  return frames;
}

uint32_t
AudioMixer::downmixToMono(const RealSamplesBuffer& in,
                          uint32_t length,
                          uint32_t numChannels,
                          RealSamplesBuffer& outMono)
{
  if (length == 0 || numChannels == 0) {
    outMono.clear();
    return 0;
  }

  const uint32_t frames = length / numChannels;
  outMono.resize(frames);

  if (frames == 0) {
    return 0;
  }

  if (numChannels == 1) {
    std::copy(in.begin(), in.begin() + frames, outMono.begin());
    return frames;
  }

  for (uint32_t f = 0; f < frames; ++f) {
    double acc = 0.0;
    const uint32_t base = f * numChannels;
    for (uint32_t c = 0; c < numChannels; ++c) {
      acc += in[base + c];
    }
    outMono[f] = static_cast<float>(acc / static_cast<double>(numChannels)) * m_gain;
  }
  return frames;
}

void
AudioMixer::outputStereoFromMono(const RealSamplesBuffer& leftMono,
                                 const RealSamplesBuffer& rightMono,
                                 uint32_t frames)
{
  if (frames == 0) {
    return;
  }

  m_stereoOutInterleaved.resize(frames * 2);
  for (uint32_t i = 0; i < frames; ++i) {
    m_stereoOutInterleaved[i * 2 + 0] = leftMono[i] * m_gain;
    m_stereoOutInterleaved[i * 2 + 1] = rightMono[i] * m_gain;
  }

  m_output.sinkAudio(m_stereoOutInterleaved,
                        static_cast<uint32_t>(m_stereoOutInterleaved.size()),
                        2);
}

void
AudioMixer::outputStereoDuplicate(const RealSamplesBuffer& mono, uint32_t frames)
{
  if (frames == 0) {
    return;
  }

  m_stereoOutInterleaved.resize(frames * 2);
  for (uint32_t i = 0; i < frames; ++i) {
    const float s = mono[i];
    m_stereoOutInterleaved[i * 2 + 0] = s * m_gain;
    m_stereoOutInterleaved[i * 2 + 1] = s * m_gain;
  }

  m_output.sinkAudio(m_stereoOutInterleaved,
                        static_cast<uint32_t>(m_stereoOutInterleaved.size()),
                        2);
}