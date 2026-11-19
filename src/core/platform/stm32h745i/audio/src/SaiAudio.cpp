#include "audio/SaiAudio.h"

#include "stm32h745i/drivers/bsp/disco/stm32h745i_discovery_audio.h"

#include <algorithm>
#include <cstring>

#include "stm32h745i_discovery.h"

#if USE_HAL_SAI_REGISTER_CALLBACKS != 0
// With register callbacks on, the BSP still forwards to BSP_AUDIO_*_CallBack, so this would work,
// but it has only been checked with the BSP's own HAL_SAI_*Callback definitions.
#warning "SaiAudio has only been checked with USE_HAL_SAI_REGISTER_CALLBACKS == 0"
#endif

static_assert(SaiAudio::sampleRateHz == AUDIO_FREQUENCY_32K, "SaiAudio initialises the BSP at 32 kHz");
static_assert(SaiAudio::outQueueFrames >= 4 * SaiAudio::dmaHalfFrames,
  "SAIAUDIO_OUT_QUEUE_FRAMES must hold several DMA halves (4 * SAIAUDIO_DMA_HALF_FRAMES at least)");
static_assert(2 * SaiAudio::dmaHalfFrames * sizeof(uint32_t) / sizeof(uint16_t) <= 0xFFFFu,
  "The BSP's DMA transfer count is 16 bits");

namespace
{
  constexpr uint32_t BSP_INSTANCE = 0;
  constexpr uint32_t MIC_STACK_WORDS = 2048 / sizeof(uint32_t);
  constexpr uint32_t MIC_THREAD_PRIORITY = 6; // Same as the ADC I/Q source thread
  constexpr float INT16_TO_FLOAT = 1.0f / 32768.0f;

  int16_t toInt16(sdrreal sample)
  {
    const float clamped = std::clamp(static_cast<float>(sample), -1.0f, 1.0f);
    return static_cast<int16_t>(clamped * 32767.0f);
  }

  uint32_t packFrame(int16_t left, int16_t right)
  {
    return static_cast<uint16_t>(left) | (static_cast<uint32_t>(static_cast<uint16_t>(right)) << 16);
  }
}

// One circular DMA buffer, shared by the two directions because only one runs at a time:
//  - Receive: 2 * dmaHalfFrames stereo frames, one 32-bit word (L, R halfwords) per frame.
//    With OUT_Init alone, Block A has slots 0 and 2 active, so the DMA takes 2 halfwords per frame.
//  - Transmit: 2 * dmaHalfFrames mono halfwords (Block B has only slot 0 active), in the first half.
// Must be in .dma_buffer (DMA_RAM, 0x30000000): DMA2 can't use the CM4's 0x1000xxxx alias of D2
// SRAM, where .bss lives. The CM4 has no data cache, so no cache maintenance is needed.
static uint32_t s_saiDmaBuffer[2 * SaiAudio::dmaHalfFrames] __attribute__((section(".dma_buffer"), aligned(32)));

SaiAudio* SaiAudio::s_active = nullptr;

SaiAudio::SaiAudio()
  : AudioOutputBase(Format{sampleRateHz, 2, 2 * sizeof(int16_t), AudioFormat::SINT16})
  , m_config()
  , m_direction(Direction::NONE)
  , m_isStarted(false)
  , m_transmit(false)
  , m_outQueue()
  , m_primeFrames(0)
  , m_priming(true)
  , m_inQueue()
  , m_micPacket()
  , m_pMicSink(nullptr)
  , m_thread(*this)
  , m_micRunning(false)
  , m_outputUnderruns(0)
  , m_droppedOutputFrames(0)
  , m_droppedMicSamples(0)
  , m_saiErrors(0)
{
}

SaiAudio::SaiAudio(SaiAudio&& other) noexcept
  : SaiAudio()
{
  m_config = other.m_config;
  m_transmit = other.m_transmit;
  m_pMicSink = other.m_pMicSink;
}

SaiAudio&
SaiAudio::operator=(SaiAudio&& other) noexcept
{
  if (this != &other) {
    stop();
    m_config = other.m_config;
    m_transmit = other.m_transmit;
    m_pMicSink = other.m_pMicSink;
  }
  return *this;
}

SaiAudio::~SaiAudio()
{
  SaiAudio::stop();
}

ResultCode
SaiAudio::configure(const Config::SaiAudio::Fields& config)
{
  m_config = config;
  m_config.outputVolume = std::min<uint32_t>(m_config.outputVolume, 100);
  m_config.inputVolume = std::min<uint32_t>(m_config.inputVolume, 100);
  return ResultCode::OK;
}

ResultCode
SaiAudio::start(uint32_t maxPacketFrames)
{
  if (m_isStarted) {
    return ResultCode::ERR_AUDIO_OUTPUT_DRIVER_ALREADY_STARTED;
  }
  if (s_active != nullptr) {
    return ResultCode::ERR_AUDIO_OUTPUT_DRIVER_ALREADY_STARTED; // Another instance owns the codec.
  }
  // Packets arrive in bursts of up to maxPacketFrames, while the DMA drains dmaHalfFrames at a time.
  // Wait for one packet plus one half before playing, so that the queue doesn't run dry just before
  // the next burst. Leave at least one half of headroom.
  m_primeFrames = std::min(maxPacketFrames + dmaHalfFrames, outQueueFrames - dmaHalfFrames);

  const ResultCode rc = m_transmit ? startMicrophone() : startSpeaker();
  m_isStarted = (rc == ResultCode::OK);
  return rc;
}

void
SaiAudio::stop()
{
  if (!m_isStarted) {
    return;
  }
  stopCurrentDirection();
  m_isStarted = false;
}

ResultCode
SaiAudio::setTransmit(bool transmit)
{
  m_transmit = transmit;
  if (!m_isStarted) {
    return ResultCode::OK;
  }
  const Direction wanted = transmit ? Direction::TRANSMIT : Direction::RECEIVE;
  if (m_direction == wanted) {
    return ResultCode::OK;
  }
  stopCurrentDirection();
  const ResultCode rc = transmit ? startMicrophone() : startSpeaker();
  if (rc != ResultCode::OK) {
    m_isStarted = false; // Nothing is running now.
  }
  return rc;
}

void
SaiAudio::stopCurrentDirection()
{
  switch (m_direction.load()) {
  case Direction::RECEIVE:
    stopSpeaker();
    break;
  case Direction::TRANSMIT:
    stopMicrophone();
    break;
  default:
    break;
  }
}

ResultCode
SaiAudio::startSpeaker()
{
  BSP_AUDIO_Init_t init{};
  init.Device        = AUDIO_OUT_DEVICE_HEADPHONE;
  init.ChannelsNbr   = 2; // Stereo, even though the demodulators are mono
  init.SampleRate    = AUDIO_FREQUENCY_32K;
  init.BitsPerSample = AUDIO_RESOLUTION_16B;
  init.Volume        = m_config.outputVolume;
  if (BSP_AUDIO_OUT_Init(BSP_INSTANCE, &init) != BSP_ERROR_NONE) {
    BSP_AUDIO_OUT_DeInit(BSP_INSTANCE);
    return ResultCode::ERR_AUDIO_OUTPUT_DRIVER_START_FAILED;
  }

  // Neither the IRQ (DMA stopped) nor addAudioData() (direction isn't RECEIVE yet) touches the queue.
  m_outQueue.clear();
  m_priming = true;
  std::memset(s_saiDmaBuffer, 0, sizeof(s_saiDmaBuffer));

  // The callbacks can fire as soon as the DMA starts.
  s_active = this;
  m_direction = Direction::RECEIVE;
  if (BSP_AUDIO_OUT_Play(BSP_INSTANCE, reinterpret_cast<uint8_t*>(s_saiDmaBuffer), sizeof(s_saiDmaBuffer))
      != BSP_ERROR_NONE) {
    m_direction = Direction::NONE;
    s_active = nullptr;
    BSP_AUDIO_OUT_DeInit(BSP_INSTANCE);
    return ResultCode::ERR_AUDIO_OUTPUT_DRIVER_START_FAILED;
  }
  return ResultCode::OK;
}

void
SaiAudio::stopSpeaker()
{
  m_direction = Direction::NONE; // addAudioData() stops queueing
  BSP_AUDIO_OUT_Stop(BSP_INSTANCE);
  BSP_AUDIO_OUT_DeInit(BSP_INSTANCE);
  s_active = nullptr;
}

ResultCode
SaiAudio::startMicrophone()
{
  BSP_AUDIO_Init_t init{};
  init.Device        = AUDIO_IN_DEVICE_ANALOG_MIC; // LINE1 input: the 3.5mm line-in jack
  init.ChannelsNbr   = 2; // As in the audio loopback test. Block B still has only slot 0 active.
  init.SampleRate    = AUDIO_FREQUENCY_32K;
  init.BitsPerSample = AUDIO_RESOLUTION_16B;
  init.Volume        = m_config.inputVolume;
  if (BSP_AUDIO_IN_Init(BSP_INSTANCE, &init) != BSP_ERROR_NONE) {
    BSP_AUDIO_IN_DeInit(BSP_INSTANCE);
    return ResultCode::ERR_AUDIO_INPUT_DRIVER_START_FAILED;
  }

  m_inQueue.clear();
  m_micRunning = true;
  if (!m_thread.start(MIC_STACK_WORDS, MIC_THREAD_PRIORITY, "SAIMIC")) {
    m_micRunning = false;
    BSP_AUDIO_IN_DeInit(BSP_INSTANCE);
    return ResultCode::ERR_AUDIO_INPUT_DRIVER_START_FAILED;
  }

  s_active = this;
  m_direction = Direction::TRANSMIT;
  // Mono halfwords: 2 * dmaHalfFrames of them, i.e. the first half of s_saiDmaBuffer.
  constexpr uint32_t micBytes = 2 * dmaHalfFrames * sizeof(int16_t);
  static_assert(micBytes <= sizeof(s_saiDmaBuffer));
  if (BSP_AUDIO_IN_Record(BSP_INSTANCE, reinterpret_cast<uint8_t*>(s_saiDmaBuffer), micBytes) != BSP_ERROR_NONE) {
    m_direction = Direction::NONE;
    s_active = nullptr;
    BSP_AUDIO_IN_Stop(BSP_INSTANCE);
    BSP_AUDIO_IN_DeInit(BSP_INSTANCE);
    m_micRunning = false;
    m_micDataAvailable.post();
    m_thread.join();
    return ResultCode::ERR_AUDIO_INPUT_DRIVER_START_FAILED;
  }
  return ResultCode::OK;
}

void
SaiAudio::stopMicrophone()
{
  BSP_AUDIO_IN_Stop(BSP_INSTANCE);
  BSP_AUDIO_IN_DeInit(BSP_INSTANCE); // Also de-initialises Block A, which clocked the input.
  m_direction = Direction::NONE;
  s_active = nullptr;

  m_micRunning = false;
  m_micDataAvailable.post();
  m_thread.join();
}

uint32_t
SaiAudio::addAudioData(const RealSamplesRef& data, uint32_t length, uint32_t numChannels)
{
  // static uint32_t counter = 0;
  // counter++;
  // if (counter > 5) {
  //   BSP_LED_Toggle(LED_RED);
  //   counter = 0;
  // }
  if (m_direction != Direction::RECEIVE || numChannels == 0) {
    return 0;
  }
  length = std::min<uint32_t>(length, static_cast<uint32_t>(data.size()));
  const uint32_t frames = length / numChannels;
  for (uint32_t f = 0; f < frames; ++f) {
    const sdrreal* frame = &data[f * numChannels];
    const int16_t left = toInt16(frame[0]);
    const int16_t right = (numChannels == 1) ? left : toInt16(frame[1]);
    if (!m_outQueue.push(packFrame(left, right))) {
      // The IRQ isn't draining as fast as the pipeline fills, e.g. because the ADC's sample clock
      // runs slightly fast relative to the SAI's. Drop the rest of this packet.
      m_droppedOutputFrames += frames - f;
      break;
    }
  }
  return length;
}

// IRQ context.
void
SaiAudio::fillOutputHalf(uint32_t* half)
{
  if (m_priming) {
    if (m_outQueue.size() < m_primeFrames) {
      std::memset(half, 0, dmaHalfFrames * sizeof(uint32_t));
      return;
    }
    m_priming = false;
  }
  for (uint32_t n = 0; n < dmaHalfFrames; ++n) {
    if (!m_outQueue.pop(half[n])) {
      // Underrun: finish this half with silence and wait for the queue to refill.
      std::memset(&half[n], 0, (dmaHalfFrames - n) * sizeof(uint32_t));
      m_outputUnderruns++;
      m_priming = true;
      return;
    }
  }
}

// IRQ context. Must finish within dmaHalfFrames sample periods, which it does easily.
void
SaiAudio::drainInputHalf(const int16_t* half)
{
  for (uint32_t n = 0; n < dmaHalfFrames; ++n) {
    if (!m_inQueue.push(half[n])) {
      m_droppedMicSamples += dmaHalfFrames - n; // The mic thread isn't keeping up.
      break;
    }
  }
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  m_micDataAvailable.postFromISR(&xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void
SaiAudio::run()
{
  while (m_micRunning) {
    if (m_inQueue.empty()) {
      m_micDataAvailable.wait();
    }
    if (!m_micRunning) {
      break;
    }
    m_micPacket.clear();
    int16_t sample;
    while (!m_micPacket.full() && m_inQueue.pop(sample)) {
      m_micPacket.push_back(static_cast<sdrreal>(sample) * INT16_TO_FLOAT);
    }
    if (!m_micPacket.empty() && m_pMicSink != nullptr) {
      m_pMicSink->sinkAudio(m_micPacket, static_cast<uint32_t>(m_micPacket.size()), 1);
    }
  }
}

void
SaiAudio::onOutputDma(bool secondHalf)
{
  SaiAudio* self = s_active;
  if (self != nullptr && self->m_direction == Direction::RECEIVE) {
    self->fillOutputHalf(&s_saiDmaBuffer[secondHalf ? dmaHalfFrames : 0]);
  }
}

void
SaiAudio::onInputDma(bool secondHalf)
{
  SaiAudio* self = s_active;
  if (self != nullptr && self->m_direction == Direction::TRANSMIT) {
    const int16_t* mic = reinterpret_cast<const int16_t*>(s_saiDmaBuffer);
    self->drainInputHalf(&mic[secondHalf ? dmaHalfFrames : 0]);
  }
}

void
SaiAudio::onSaiError()
{
  SaiAudio* self = s_active;
  if (self != nullptr) {
    self->m_saiErrors++;
  }
}

// BSP audio callbacks. The BSP defines these weakly; these definitions replace them.
// They are called from the SAI2 DMA IRQ handlers (DMA2_Stream1 out, DMA2_Stream4 in) in setup3's
// stm32h7xx_it.c, at BSP_AUDIO_OUT/IN_IT_PRIORITY. Those are below
// configMAX_SYSCALL_INTERRUPT_PRIORITY, so FreeRTOS FromISR calls are allowed.
// Nothing else may define them. The old audio loopback test did, so it is out of the build.
extern "C" {

void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t /*Instance*/)
{
  SaiAudio::onOutputDma(false);
}

void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t /*Instance*/)
{
  SaiAudio::onOutputDma(true);
}

void BSP_AUDIO_OUT_Error_CallBack(uint32_t /*Instance*/)
{
  SaiAudio::onSaiError();
}

void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t /*Instance*/)
{
  SaiAudio::onInputDma(false);
}

void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t /*Instance*/)
{
  SaiAudio::onInputDma(true);
}

void BSP_AUDIO_IN_Error_CallBack(uint32_t /*Instance*/)
{
  SaiAudio::onSaiError();
}

} // extern "C"
