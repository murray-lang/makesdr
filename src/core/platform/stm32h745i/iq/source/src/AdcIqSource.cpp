#include "iq/source/AdcIqSource.h"
#include "stm32h745i/setup/AdcConfig.h"
#include "stm32h745i/setup/config.h"

#define USE_ADC_INTERRUPT_MODE
// Conversion factor from 16-bit ADC to float [-1.0, 1.0]
constexpr float ADC_TO_FLOAT = 1.0f / 32768.0f;

AdcIqSource::AdcIqSource(IqSink* pSink)
  : IqSource(pSink)
  , m_thread(*this)
  , m_pConfig(getAdcConfig())
  , m_pSink(pSink)
  , m_iqBuffer()
  // , m_adcBufferI{0}
  // , m_adcBufferQ{0}
  // , m_writeIndex(0)
  // , m_bufferHalfFull(false)
  , m_isRunning(false)
  , m_maxPacketSamples(0)
  , m_numCurrentSamples(0)
{
  m_pConfig->sampleRate = SAMPLE_RATE;
}

AdcIqSource::AdcIqSource(AdcIqSource&& other) noexcept
  : IqSource(std::move(other))
  , m_thread(*this)
  , m_pConfig(other.m_pConfig)
  , m_pSink(other.m_pSink)
  , m_isRunning(false)
  , m_maxPacketSamples(other.m_maxPacketSamples)
  , m_numCurrentSamples(0)
{
  other.m_pSink = nullptr;
}

AdcIqSource::~AdcIqSource()
{
  AdcIqSource::stop();
}

AdcIqSource&
AdcIqSource::operator=(AdcIqSource&& other) noexcept
{
  IqSource::operator=(std::move(other));
  m_pSink = other.m_pSink;
  other.m_pSink = nullptr;
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
  return ResultCode::OK;
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
  m_maxPacketSamples = maxPacketFrames;
  if (m_isRunning) {
    return ResultCode::ERR_AUDIO_INPUT_DRIVER_ALREADY_STARTED;
  }
  m_isRunning = true;
  m_thread.start(STACK_SIZE, THREAD_PRIORITY, "ADCIQ");

  ResultCode rc;

#ifdef USE_ADC_INTERRUPT_MODE
  // Reset circular buffer for interrupt mode
  m_iqBuffer.clear();
  rc = startInterrupts();
#else
  rc = startDma();
#endif

  if (rc != ResultCode::OK) return rc;

  rc = startTimer();
  if (rc != ResultCode::OK) return rc;
  m_isRunning = true;
  return ResultCode::OK;
}

void AdcIqSource::stop()
{
  if (!m_isRunning)
    return;

  HAL_TIM_Base_Stop(&m_pConfig->timer);

#ifdef USE_ADC_INTERRUPT_MODE
  stopInterrupts();
#else
  stopDma();
#endif

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
  ResultCode rc = MX_ADC1_Init(&m_pConfig->I, &m_pConfig->multimode);
  if (rc != ResultCode::OK) return rc;
  return MX_ADC2_Init(&m_pConfig->Q);
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

// ResultCode AdcIqSource::startDma()
// {
//   // In dual mode, only start ADC1 (master)
//   // Data from both ADCs comes through ADC1's DMA
//   if (HAL_ADC_Start_DMA(&m_pConfig->I.adc, (uint32_t*)m_adcBufferI, ADC_BUFFER_SIZE * 2) != HAL_OK) {
//     return ResultCode::ERR_DMA_START;
//   }
//   return ResultCode::OK;
// }
//
// void
// AdcIqSource::stopDma()
// {
//   HAL_ADC_Stop_DMA(&m_pConfig->I.adc);
// }

ResultCode
AdcIqSource::startInterrupts()
{
  // In dual mode, only start ADC1 (master)
  // ADC2 is triggered automatically in simultaneous mode
  if (HAL_ADC_Start_IT(&m_pConfig->I.adc) != HAL_OK){
    return ResultCode::ERR_ADC_START;
  }
  return ResultCode::OK;
}

void
AdcIqSource::stopInterrupts()
{
  HAL_ADC_Stop_IT(&m_pConfig->I.adc);
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

// void AdcIqSource::processBuffer(uint32_t offset)
// {
//   // Convert ADC samples to float and interleave I/Q
//   RealSamplesMax samples;
//   // samples.resize(m_config.bufferSize * 2); // I and Q interleaved
//
//   for (uint32_t i = 0; i < ADC_BUFFER_SIZE; ++i) {
//     // Convert from unsigned 16-bit to signed float [-1.0, 1.0]
//     float iSample = (static_cast<float>(m_adcBufferI[offset + i]) - 32768.0f) * ADC_TO_FLOAT;
//     float qSample = (static_cast<float>(m_adcBufferQ[offset + i]) - 32768.0f) * ADC_TO_FLOAT;
//
//     samples[i * 2] = iSample;
//     samples[i * 2 + 1] = qSample;
//   }
//
//   // Send to sink
//   if (m_pSink) {
//     m_pSink->sinkIq(samples, ADC_BUFFER_SIZE * 2, 2);
//   }
// }

// // DMA mode callbacks
// void AdcIqSource::onAdcConversionComplete()
// {
//   // Process second half of buffer
//   processBuffer(ADC_BUFFER_SIZE);
// }
//
// void AdcIqSource::onAdcConversionHalfComplete()
// {
//   // Process first half of buffer
//   processBuffer(0);
// }

// Interrupt mode callback - dual simultaneous mode
void AdcIqSource::onAdcDualConversion(uint16_t valueI, uint16_t valueQ)
{
  if (m_iqBuffer.full()) {
    return;
  }
  // Convert from unsigned 16-bit to signed float [-1.0, 1.0]
  float iSample = (static_cast<float>(valueI) - 32768.0f) * ADC_TO_FLOAT;
  float qSample = (static_cast<float>(valueQ) - 32768.0f) * ADC_TO_FLOAT;

  // Create complex sample and insert into circular buffer
  sdrcomplex sample(iSample, qSample);

  m_iqBuffer.push(sample);
    // BSP_LED_On(LED_RED);

  if (m_iqBuffer.size() > m_maxPacketSamples/2) {
    BaseType_t xHigherPriorityTaskWoken;
    m_dataAvailable.postFromISR(&xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }

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
      m_pSink->sinkIq(m_buffer, m_numCurrentSamples);
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