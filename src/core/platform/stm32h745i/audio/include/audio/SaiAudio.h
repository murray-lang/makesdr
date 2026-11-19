#ifndef STM32H745_SAI_AUDIO_H
#define STM32H745_SAI_AUDIO_H

#include <ResultCode.h>
#include <audio/AudioOutputBase.h>
#include <audio/AudioSink.h>
#include <samples/SampleTypes.h>
#include <config/struct/SaiAudioConfig.h>

#include <thread/Runnable.h>
#include <thread/Semaphore.h>
#include <thread/Thread.h>

#include <etl/queue_spsc_atomic.h>
#include <etl/vector.h>
#include <atomic>
#include <cstdint>

// Half-duplex audio through the STM32H745I-DISCO's WM8994 codec, using the BSP audio driver:
//  - Receive: 16-bit stereo to the headphone/line-out jack (SAI2 Block A, slots 0 and 2).
//  - Transmit: line-in (SAI2 Block B, slot 0, mono) to the mic sink. The mic goes through an
//    external amplifier into the line-in jack.
// Only one direction runs at a time. setTransmit() switches by stopping the codec and
// re-initialising it for the other direction, because a full-duplex codec setup isn't needed.
//
// DMA half/complete interrupts arrive through the BSP's BSP_AUDIO_OUT_* and BSP_AUDIO_IN_*
// callbacks, which SaiAudio.cpp defines and forwards to the active instance. There is one codec,
// so at most one SaiAudio can be started.
class SaiAudio : public AudioOutputBase, public Runnable
{
public:
  // Fixed for now. It must be in the 48 kHz family, because the SAI2 kernel clock is the CM7's
  // PLL2P (see MX_SAI2_ClockConfig() in setup3).
  static constexpr uint32_t sampleRateHz = 32000;

  // Frames per half of the circular DMA buffer. The DMA IRQ fires once per half.
  static constexpr uint32_t dmaHalfFrames = SAIAUDIO_DMA_HALF_FRAMES;
  // Stereo frames queued between the pipeline thread and the output DMA IRQ.
  static constexpr uint32_t outQueueFrames = SAIAUDIO_OUT_QUEUE_FRAMES;
  // Mono samples queued between the input DMA IRQ and the mic thread.
  static constexpr uint32_t inQueueSamples = 4 * dmaHalfFrames;

  SaiAudio();
  // Moves only the configuration. Neither instance may be started.
  SaiAudio(SaiAudio&& other) noexcept;
  ~SaiAudio() override;

  SaiAudio(const SaiAudio&) = delete;
  SaiAudio& operator=(const SaiAudio&) = delete;
  SaiAudio& operator=(SaiAudio&& other) noexcept;

  ResultCode configure(const Config::SaiAudio::Fields& config);

  // Starts the codec in the current direction: receive (speaker) unless setTransmit(true) was
  // called. maxPacketFrames is the largest packet addAudioData() will be given. It sets how much
  // output is buffered before playback begins.
  ResultCode start(uint32_t maxPacketFrames) override;
  void stop() override;

  // Pipeline thread. data holds `length` interleaved samples of `numChannels` channels. Mono is
  // copied to both channels; channels beyond the second are ignored. Discarded unless receiving.
  uint32_t addAudioData(const RealSamplesRef& data, uint32_t length, uint32_t numChannels) override;

  // Half-duplex PTT: true = line-in to the mic sink, false = speaker. If not started, this only
  // selects the direction that start() will use. Call it from the same task as start()/stop().
  ResultCode setTransmit(bool transmit);
  [[nodiscard]] bool isTransmitting() const { return m_direction == Direction::TRANSMIT; }

  // Receives mic audio (mono, sampleRateHz) on SaiAudio's own thread while transmitting.
  void setMicSink(AudioSink* pSink) { m_pMicSink = pSink; }

  // Diagnostics
  [[nodiscard]] uint32_t outputUnderruns() const { return m_outputUnderruns; }      // DMA half short of data
  [[nodiscard]] uint32_t droppedOutputFrames() const { return m_droppedOutputFrames; } // Output queue full
  [[nodiscard]] uint32_t droppedMicSamples() const { return m_droppedMicSamples; }   // Mic queue full
  [[nodiscard]] uint32_t saiErrors() const { return m_saiErrors; }

  // Mic thread: delivers queued line-in samples to the mic sink.
  void run() override;

  // DMA IRQ context only: called by the BSP callbacks in SaiAudio.cpp.
  static void onOutputDma(bool secondHalf);
  static void onInputDma(bool secondHalf);
  static void onSaiError();

private:
  enum class Direction : uint8_t { NONE, RECEIVE, TRANSMIT };

  ResultCode startSpeaker();
  void stopSpeaker();
  ResultCode startMicrophone();
  void stopMicrophone();
  void stopCurrentDirection();

  void fillOutputHalf(uint32_t* half);         // IRQ context
  void drainInputHalf(const int16_t* half);    // IRQ context

  static SaiAudio* s_active;

  Config::SaiAudio::Fields m_config;
  std::atomic<Direction> m_direction;
  bool m_isStarted;
  bool m_transmit;

  // Output: L in bits [15:0], R in bits [31:16], i.e. the halfword order the DMA sends.
  etl::queue_spsc_atomic<uint32_t, outQueueFrames> m_outQueue;
  uint32_t m_primeFrames;   // Queue level that ends priming
  bool m_priming;           // IRQ only (and before DMA starts): output silence until primed

  // Input
  etl::queue_spsc_atomic<int16_t, inQueueSamples> m_inQueue;
  etl::vector<sdrreal, dmaHalfFrames> m_micPacket;
  AudioSink* m_pMicSink;
  Thread m_thread;
  Semaphore m_micDataAvailable;
  std::atomic<bool> m_micRunning;

  std::atomic<uint32_t> m_outputUnderruns;
  std::atomic<uint32_t> m_droppedOutputFrames;
  std::atomic<uint32_t> m_droppedMicSamples;
  std::atomic<uint32_t> m_saiErrors;
};

#endif // STM32H745_SAI_AUDIO_H
