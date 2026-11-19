#pragma once
#include "AudioMixerInput.h"

#include <cstddef>
#include <etl/vector.h>

// Mixes the audio of up to two RX pipelines into one interleaved stereo stream:
//   - input B disabled: input A is duplicated to both channels and passed straight through.
//   - input B enabled:  A -> left, B -> right. Each pair is emitted once both inputs have delivered
//                       a packet. If one input delivers twice in a row, its older packet is flushed
//                       to both channels so that a stalled input never blocks the other.
//
// All inputs must be fed from the same thread (the IQ sink thread). setInputBEnabled() may be
// called from another thread; it only toggles a flag that the audio path reacts to.
//
// The mixing logic is non-template (AudioMixerBase, in AudioMixer.cpp). AudioMixerT<MaxFrames>
// only supplies the per-input buffer storage, so memory-constrained targets can shrink it.
class AudioMixerBase
{
public:
  AudioMixerInput& inputA() { return m_inputA; }
  AudioMixerInput& inputB() { return m_inputB; }

  void setInputBEnabled(bool enabled) { m_inputBEnabled = enabled; }
  [[nodiscard]] bool inputBEnabled() const { return m_inputBEnabled; }

  uint32_t sinkAudioInput(
    AudioMixerInput::InputId inputId,
    const RealSamplesRef& samples,
    uint32_t length,
    uint32_t numChannels
    );

protected:
  // The buffers are owned by the derived class (see AudioMixerT) and must outlive this object.
  AudioMixerBase(AudioSink& output, RealSamplesRef& pendingA, RealSamplesRef& pendingB, RealSamplesRef& stereoOut);

  uint32_t downmixToMono(const RealSamplesRef& in,
                         uint32_t length,
                         uint32_t numChannels,
                         RealSamplesRef& outMono) const;

  void outputStereoFromPair(const RealSamplesRef& leftMono,
                            const RealSamplesRef& rightMono);

  void outputStereoFromSingle(const RealSamplesRef& mono);

private:
  AudioMixerInput m_inputA;
  AudioMixerInput m_inputB;
  AudioSink& m_output;

  volatile bool m_inputBEnabled;

  bool m_havePendingA;
  bool m_havePendingB;
  RealSamplesRef& m_pendingA;
  RealSamplesRef& m_pendingB;
  RealSamplesRef& m_stereoOutInterleaved;
  sdrreal m_gain;
};

// Storage is a separate base, listed before AudioMixerBase, so the buffers are fully constructed
// before AudioMixerBase binds references to them (base-from-member idiom).
template<size_t MaxFrames>
struct AudioMixerStorage
{
  etl::vector<sdrreal, MaxFrames> m_storageA;
  etl::vector<sdrreal, MaxFrames> m_storageB;
  etl::vector<sdrreal, MaxFrames * 2> m_storageStereoOut;
};

// MaxFrames: maximum mono frames per input packet. Larger packets are truncated.
// Memory: 4 * MaxFrames * sizeof(sdrreal) (two mono inputs plus the interleaved stereo output).
template<size_t MaxFrames>
class AudioMixerT : private AudioMixerStorage<MaxFrames>, public AudioMixerBase
{
  static_assert(MaxFrames > 0, "AudioMixerT: MaxFrames must be non-zero");

public:
  explicit AudioMixerT(AudioSink& output)
    : AudioMixerStorage<MaxFrames>()
    , AudioMixerBase(output, this->m_storageA, this->m_storageB, this->m_storageStereoOut)
  {
  }
};

// AUDIO_MIXER_MAX_FRAMES is set by the AUDIO_MIXER_MAX_FRAMES CMake cache variable
// (see src/core/common/audio/mixer/CMakeLists.txt).
#ifndef AUDIO_MIXER_MAX_FRAMES
#error "AUDIO_MIXER_MAX_FRAMES must be defined (link against core-common-audio-mixer)"
#endif

using AudioMixer = AudioMixerT<AUDIO_MIXER_MAX_FRAMES>;
