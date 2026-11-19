#include "iq/source/AdcIqSource.h"
#include "stm32h745i/AdcConfig.h"
#include "stm32h745i/config.h"

// Conversion factor from 16-bit ADC to float [-1.0, 1.0]
constexpr float ADC_TO_FLOAT = 1.0f / 32768.0f;

#if USE_HAL_ADC_REGISTER_CALLBACKS != 1
#error "AdcIqSource registers its own ADC callbacks: set USE_HAL_ADC_REGISTER_CALLBACKS to 1U in stm32h7xx_hal_conf.h"
#endif

static_assert(ADC_BUFFER_SIZE >= 2 * ADC_DMA_HALF_FRAMES,
  "ADC_BUFFER_SIZE (sample queue) must hold at least one full DMA buffer (2 * ADC_DMA_HALF_FRAMES)");

// Dual regular-simultaneous mode with DAMDF = 32_10_BITS: each 32-bit word read from ADC12_CDR
// holds master (ADC1 = I) in bits [15:0] and slave (ADC2 = Q) in bits [31:16].
// Circular: the first half is ready at the half-transfer callback, the second at transfer complete.
// Must be in .dma_buffer (DMA_RAM, 0x30000000): DMA1 can't use the CM4's 0x1000xxxx alias of D2
// SRAM, where .bss lives. The CM4 has no data cache, so no cache maintenance is needed.
static uint32_t s_adcDmaBuffer[2 * ADC_DMA_HALF_FRAMES] __attribute__((section(".dma_buffer"), aligned(32)));

AdcIqSource* AdcIqSource::s_active = nullptr;

AdcIqSource::AdcIqSource(IqSink* pSink)
  : IqSource(pSink)
  , m_thread(*this)
  , m_pConfig(getAdcConfig())
  // , m_pSink(pSink)
  , m_iqBuffer()
  // , m_adcBufferI{0}
  // , m_adcBufferQ{0}
  // , m_writeIndex(0)
  // , m_bufferHalfFull(false)
  , m_isRunning(false)
  , m_droppedSamples(0)
  , m_adcErrors(0)
  , m_maxPacketSamples(0)
  , m_numCurrentSamples(0)
{
  m_pConfig->sampleRate = SAMPLE_RATE;
}

AdcIqSource::AdcIqSource(AdcIqSource&& other) noexcept
  : IqSource(std::move(other))
  , m_thread(*this)
  , m_pConfig(other.m_pConfig)
  // , m_pSink(other.m_pSink)
  , m_isRunning(false)
  , m_droppedSamples(0)
  , m_adcErrors(0)
  , m_maxPacketSamples(other.m_maxPacketSamples)
  , m_numCurrentSamples(0)
{
  other.m_pIqSink = nullptr;
}

AdcIqSource::~AdcIqSource()
{
  AdcIqSource::stop();
}

AdcIqSource&
AdcIqSource::operator=(AdcIqSource&& other) noexcept
{
  IqSource::operator=(std::move(other));
  m_pIqSink = other.m_pIqSink;
  other.m_pIqSink = nullptr;
  m_pConfig = other.m_pConfig;
  m_maxPacketSamples = other.m_maxPacketSamples;
  m_numCurrentSamples = 0;
  m_isRunning = false;
  return *this;
}

ResultCode
AdcIqSource::configure(const Config::AdcIqSource::Fields& config)
{
  if (config.sampleRate != 0) {
    m_pConfig->sampleRate = config.sampleRate;
  } else {
    m_pConfig->sampleRate = SAMPLE_RATE;
  }
  return initialise();
}

ResultCode
AdcIqSource::initialise()
{
  ResultCode rc = initialiseAdcs();
  if (rc != ResultCode::OK) return rc;

  return configureTimer();
}

ResultCode
AdcIqSource::start(uint32_t maxPacketFrames)
{
  if (m_isRunning) {
    return ResultCode::ERR_AUDIO_INPUT_DRIVER_ALREADY_STARTED;
  }
  // Reset the queue and packet state before anything can produce or consume.
  m_iqBuffer.clear();
  m_maxPacketSamples = std::min<uint32_t>(maxPacketFrames, static_cast<uint32_t>(m_buffer.capacity()));
  m_buffer.resize(m_maxPacketSamples);
  m_numCurrentSamples = 0;
  m_droppedSamples = 0;
  m_adcErrors = 0;

  m_isRunning = true;
  if (!m_thread.start(STACK_SIZE/sizeof(uint32_t), THREAD_PRIORITY, "ADCIQ")) {
    m_isRunning = false;
    return ResultCode::ERR_IQ_THREAD_START_FAILED;
  }

  ResultCode rc = startDma();
  if (rc == ResultCode::OK) {
    rc = startTimer();
  }
  if (rc != ResultCode::OK) {
    stop(); // Stops whatever started and joins the thread.
  }
  return rc;
}

void AdcIqSource::stop()
{
  if (!m_isRunning)
    return;

  HAL_TIM_Base_Stop(&m_pConfig->timer);
  stopDma();

  m_isRunning = false;
  m_dataAvailable.post();
  m_thread.join();
}

ResultCode
AdcIqSource::initialiseAdcs()
{
  ResultCode rc = configureAdcs();
  if (rc != ResultCode::OK) return rc;
  return calibrateAdcs();
}

ResultCode
AdcIqSource::configureAdcs()
{
  ResultCode rc = ADC1_Init(&m_pConfig->I, &m_pConfig->multimode);
  if (rc != ResultCode::OK) return rc;
  return ADC2_Init(&m_pConfig->Q);
}

ResultCode
AdcIqSource::calibrateAdcs()
{
  // Calibrate ADCs
  if (HAL_ADCEx_Calibration_Start(&m_pConfig->I.adc, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
  {
    return ResultCode::ERR_ADC_CALIBRATION;
  }
  if (HAL_ADCEx_Calibration_Start(&m_pConfig->Q.adc, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
  {
    return ResultCode::ERR_ADC_CALIBRATION;
  }
  return ResultCode::OK;
}

ResultCode
AdcIqSource::startDma()
{
  ADC_HandleTypeDef* master = &m_pConfig->I.adc;
  if (master->DMA_Handle == nullptr) {
    return ResultCode::ERR_DMA_START; // HAL_ADC_MspInit didn't link a DMA stream to the master.
  }
  if (HAL_ADC_RegisterCallback(master, HAL_ADC_CONVERSION_HALF_CB_ID, &AdcIqSource::onDmaHalfComplete) != HAL_OK
   || HAL_ADC_RegisterCallback(master, HAL_ADC_CONVERSION_COMPLETE_CB_ID, &AdcIqSource::onDmaComplete) != HAL_OK
   || HAL_ADC_RegisterCallback(master, HAL_ADC_ERROR_CB_ID, &AdcIqSource::onAdcError) != HAL_OK) {
    return ResultCode::ERR_DMA_START;
  }
  s_active = this;

  // Only the master is started: ADC2 converts simultaneously and both results come through
  // ADC12_CDR on the master's DMA stream. Conversions begin on the first TIM6 TRGO.
  if (HAL_ADCEx_MultiModeStart_DMA(master, s_adcDmaBuffer, 2 * ADC_DMA_HALF_FRAMES) != HAL_OK) {
    s_active = nullptr;
    return ResultCode::ERR_DMA_START;
  }
  return ResultCode::OK;
}

void
AdcIqSource::stopDma()
{
  ADC_HandleTypeDef* master = &m_pConfig->I.adc;
  HAL_ADCEx_MultiModeStop_DMA(master);
  s_active = nullptr;
  HAL_ADC_UnRegisterCallback(master, HAL_ADC_CONVERSION_HALF_CB_ID);
  HAL_ADC_UnRegisterCallback(master, HAL_ADC_CONVERSION_COMPLETE_CB_ID);
  HAL_ADC_UnRegisterCallback(master, HAL_ADC_ERROR_CB_ID);
}

uint32_t
AdcIqSource::getTimerClockFreq()
{
  // Get the APB1 timer clock frequency
  uint32_t apb1TimerClock;

  // Get APB1 peripheral clock
  uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();

  // Timer clock is 2x APB clock if APB prescaler != 1, otherwise same as APB clock
  RCC_ClkInitTypeDef clkConfig;
  uint32_t flashLatency;
  HAL_RCC_GetClockConfig(&clkConfig, &flashLatency);

  if (clkConfig.APB1CLKDivider == RCC_APB1_DIV1)
    apb1TimerClock = pclk1;
  else
    apb1TimerClock = pclk1 * 2;

  return apb1TimerClock;
}

ResultCode
AdcIqSource::configureTimer()
{
  if (m_pConfig->sampleRate == 0) {
    return ERR_ADC_SAMPLE_RATE_NOT_SET;
  }

  // Enable TIM6 clock
  __HAL_RCC_TIM6_CLK_ENABLE();

  const uint32_t timerClock = getTimerClockFreq();

  uint32_t period = (timerClock / m_pConfig->sampleRate) - 1;

  m_pConfig->timer.Instance = TIM6;
  m_pConfig->timer.Init.Prescaler = 0;
  m_pConfig->timer.Init.CounterMode = TIM_COUNTERMODE_UP;
  m_pConfig->timer.Init.Period = period;
  m_pConfig->timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if (HAL_TIM_Base_Init(&m_pConfig->timer) != HAL_OK) {
    return ResultCode::ERR_ADC_TIMER_CONFIG;
  }

  // Configure TRGO to update event
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(&m_pConfig->timer, &sMasterConfig) != HAL_OK) {
    return ResultCode::ERR_ADC_TIMER_CONFIG;
  }
  return ResultCode::OK;
}

ResultCode AdcIqSource::startTimer()
{
  // Ensure TIM6 clock is enabled (might be gated after STOP mode)
  __HAL_RCC_TIM6_CLK_ENABLE();

  // Timer should already be configured to generate TRGO events at the sample rate
  // Start the timer manually (bypass HAL if it hangs)
  __HAL_TIM_ENABLE(&m_pConfig->timer);

  return ResultCode::OK;
}

void
AdcIqSource::onDmaHalfComplete(ADC_HandleTypeDef* /*hadc*/)
{
  if (s_active != nullptr) {
    s_active->processDmaHalf(&s_adcDmaBuffer[0]);
  }
}

void
AdcIqSource::onDmaComplete(ADC_HandleTypeDef* /*hadc*/)
{
  if (s_active != nullptr) {
    s_active->processDmaHalf(&s_adcDmaBuffer[ADC_DMA_HALF_FRAMES]);
  }
}

void
AdcIqSource::onAdcError(ADC_HandleTypeDef* /*hadc*/)
{
  if (s_active != nullptr) {
    s_active->m_adcErrors++;
  }
}

// IRQ context. Converts one half of the DMA buffer into the queue, then wakes run().
// The DMA is filling the other half meanwhile, so this must finish within
// ADC_DMA_HALF_FRAMES sample periods (it takes a few microseconds).
void
AdcIqSource::processDmaHalf(const uint32_t* half)
{
  uint32_t dropped = 0;
  for (uint32_t n = 0; n < ADC_DMA_HALF_FRAMES; ++n) {
    const uint32_t word = half[n];
    const float iSample = (static_cast<float>(word & 0xFFFFu) - 32768.0f) * ADC_TO_FLOAT;
    const float qSample = (static_cast<float>(word >> 16) - 32768.0f) * ADC_TO_FLOAT;
    if (!m_iqBuffer.push(sdrcomplex(iSample, qSample))) {
      dropped += ADC_DMA_HALF_FRAMES - n; // run() isn't keeping up: drop the rest of this half.
      break;
    }
  }
  if (dropped != 0) {
    m_droppedSamples += dropped;
    // static uint32_t counter = 0;
    // counter++;
    // if (counter > 5) {
    //   BSP_LED_Toggle(LED_RED);
    //   counter = 0;
    // }
  } else {
    static uint32_t counter = 0;
    counter++;
    if (counter > 5) {
      BSP_LED_Toggle(LED_GREEN);
      counter = 0;
    }
  }

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  m_dataAvailable.postFromISR(&xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void
AdcIqSource::run()
{

  while (m_isRunning) {
    {

      if (m_iqBuffer.empty()) {
        m_dataAvailable.wait();
      }
      if (!m_isRunning) {

        break;
      }

      if (!m_iqBuffer.empty()) {

        uint32_t requiredSamples = m_maxPacketSamples - m_numCurrentSamples;
        uint32_t numIncomingSamples = m_iqBuffer.size();
        uint32_t samplesToRead = std::min(requiredSamples, numIncomingSamples);
        getSamplesFromBuffer(samplesToRead);
      }
    }
    if (m_numCurrentSamples == m_maxPacketSamples) {

      // static uint32_t counter = 0;
      // counter++;
      // if (counter > 10) {
      //   BSP_LED_Toggle(LED_RED);
      //   counter = 0;
      // }
      if (m_pIqSink) {
        m_pIqSink->sinkIq(m_buffer, m_numCurrentSamples);
      }

      m_numCurrentSamples = 0;
    }
  }
}

void
AdcIqSource::getSamplesFromBuffer(size_t numSamples)
{
  for (size_t i = 0; i < numSamples; i++) {
    m_buffer.at(m_numCurrentSamples++) = m_iqBuffer.front();
    m_iqBuffer.pop();
  }
}