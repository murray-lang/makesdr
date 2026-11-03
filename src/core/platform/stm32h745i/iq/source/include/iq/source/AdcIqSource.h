#ifndef STM32H745_ADC_IQ_SOURCE_H
#define STM32H745_ADC_IQ_SOURCE_H

#include <ResultCode.h>
#include <CrossPlatformTypes.h>
#include <iq/base/IqSource.h>
#include <stm32h745i/setup/AdcConfig.h>
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

  // Called by interrupt handlers (choose one mode)
  void onAdcDualConversion(uint16_t valueI, uint16_t valueQ);  // Interrupt mode - dual simultaneous
  // void onAdcConversionComplete();      // DMA mode - full buffer
  // void onAdcConversionHalfComplete();  // DMA mode - half buffer

  void run() override;

private:
  ResultCode initialiseAdcs();
  ResultCode configureAdcs();
  ResultCode calibrateAdcs();
  // ResultCode startDma();
  // void stopDma();
  ResultCode startInterrupts();
  void stopInterrupts();
  ResultCode configureTimer();
  uint32_t getTimerClockFreq();
  ResultCode startTimer();
  // void processBuffer(uint32_t offset); // DMA mode

  void getSamplesFromBuffer(size_t numSamples);

  Thread m_thread;
  AdcConfig* m_pConfig;
  IqSink* m_pSink;

  // Circular buffer for converted complex samples
  etl::queue_spsc_atomic<sdrcomplex, ADC_BUFFER_SIZE> m_iqBuffer;
  ComplexSamplesBuffer m_buffer;

  // Double buffering for DMA mode only
  // uint16_t m_adcBufferI[ADC_BUFFER_SIZE*2];
  // uint16_t m_adcBufferQ[ADC_BUFFER_SIZE*2];

  // // For interrupt mode
  // volatile uint32_t m_writeIndex;  // Current write position in buffer
  // volatile bool m_bufferHalfFull;  // Flag for half-buffer processing

  ::atomic<bool> m_isRunning;
  // Mutex m_mutex;
  Semaphore m_dataAvailable;
  uint32_t m_maxPacketSamples;
  uint32_t m_numCurrentSamples;
};

#endif // STM32H745_ADC_IQ_SOURCE_H