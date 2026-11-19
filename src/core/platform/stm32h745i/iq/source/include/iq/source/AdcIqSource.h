#ifndef STM32H745_ADC_IQ_SOURCE_H
#define STM32H745_ADC_IQ_SOURCE_H

#include <ResultCode.h>
#include <CrossPlatformTypes.h>
#include <iq/base/IqSource.h>
#include <stm32h745i/AdcConfig.h>
#include <samples/SampleTypes.h>
#include <etl/queue_spsc_atomic.h>

#include <thread/Runnable.h>
#include <thread/Semaphore.h>
#include <thread/Thread.h>

#include <config/struct/AdcIqSourceConfig.h>


class AdcIqSource : public IqSource, public Runnable
{
public:
  AdcIqSource() : AdcIqSource(nullptr) {}
  AdcIqSource(IqSink* pSink);
  AdcIqSource(AdcIqSource&& other) noexcept;
  ~AdcIqSource() override;

  AdcIqSource& operator=(AdcIqSource&& other) noexcept;

  ResultCode configure(const Config::AdcIqSource::Fields& config);

  ResultCode initialise();
  ResultCode start(uint32_t maxPacketFrames) override;
  void stop() override;

  uint32_t getSampleRate() const override { return m_pConfig->sampleRate; };

  // Diagnostics: samples dropped because the queue was full, and ADC/DMA error callbacks seen.
  [[nodiscard]] uint32_t droppedSamples() const { return m_droppedSamples; }
  [[nodiscard]] uint32_t adcErrors() const { return m_adcErrors; }

  void run() override;

private:
  // HAL callbacks, registered on the ADC master handle in startDma() (requires
  // USE_HAL_ADC_REGISTER_CALLBACKS). They run in DMA IRQ context and forward to s_active.
  static void onDmaHalfComplete(ADC_HandleTypeDef* hadc);
  static void onDmaComplete(ADC_HandleTypeDef* hadc);
  static void onAdcError(ADC_HandleTypeDef* hadc);
  void processDmaHalf(const uint32_t* half); // IRQ context

  // There is one I/Q ADC pair, so at most one source can be running.
  static AdcIqSource* s_active;

  ResultCode initialiseAdcs();
  ResultCode configureAdcs();
  ResultCode calibrateAdcs();
  ResultCode startDma();
  void stopDma();
  ResultCode configureTimer();
  uint32_t getTimerClockFreq();
  ResultCode startTimer();

  void getSamplesFromBuffer(size_t numSamples);

  Thread m_thread;
  AdcConfig* m_pConfig;
  // IqSink* m_pSink;

  // Converted complex samples: produced by the DMA IRQ, consumed by run().
  etl::queue_spsc_atomic<sdrcomplex, ADC_BUFFER_SIZE> m_iqBuffer;
  ComplexSamplesBuffer m_buffer;

  ::atomic<bool> m_isRunning;
  ::atomic<uint32_t> m_droppedSamples;
  ::atomic<uint32_t> m_adcErrors;
  // Mutex m_mutex;
  Semaphore m_dataAvailable;
  uint32_t m_maxPacketSamples;
  uint32_t m_numCurrentSamples;
};

#endif // STM32H745_ADC_IQ_SOURCE_H