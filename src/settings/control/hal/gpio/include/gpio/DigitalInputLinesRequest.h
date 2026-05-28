#pragma once

#include "CrossPlatformTypes.h"
#include "thread/Thread.h"
#include <thread/Mutex.h>
#include <thread/Runnable.h>
#include <etl/unordered_map.h>

#include "base/Gpio.h"
#include "base/GpioEventQueue.h"
#include "base/GpioInputLines.h"
#include "base/GpioInputLinesRequest.h"
#include "base/GpioLinesRequestBase.h"
#include "config/GpioInputLinesConfig.h"


class DigitalInputLinesRequest : public GpioInputLinesRequest, public Runnable
{

public:

  struct LineState
  {
    uint32_t line;
    bool debounce;
    bool isDebounced;
    // uint8_t candidateValue;
    // uint64_t candidateEdgeTime;
    uint64_t firstEdgeTime;
    uint64_t lastRisingTime;
    uint64_t lastFallingTime;
    uint8_t value;
    bool changed;
    bool processed;
  };

#ifdef USE_ETL
  using LineStateMap = etl::unordered_map<uint32_t, LineState, MAX_GPIO>;
  using LineStateVector = etl::vector<LineState, MAX_GPIO>;
#else
  using LineStateMap = std::unordered_map<uint32_t, LineState>;
  using LineStateVector = std::vector<LineState>;
#endif

  class Callback
  {
  public:
    virtual ~Callback() = default;
    virtual void callback(LineStateVector& lineStates) = 0;

  };

  explicit DigitalInputLinesRequest();

  // Delete copy operations
  DigitalInputLinesRequest(const DigitalInputLinesRequest&) = delete;
  DigitalInputLinesRequest& operator=(const DigitalInputLinesRequest&) = delete;

  // Move constructor
  DigitalInputLinesRequest(DigitalInputLinesRequest&& other) noexcept;

  // Move assignment operator
  DigitalInputLinesRequest& operator=(DigitalInputLinesRequest&& other) noexcept;

  ~DigitalInputLinesRequest() override;

  void initialise(void* gpioHandle, const char * context) override;
  void run() override;

  ResultCode startCallbacks(Callback& callback);
  void stopCallbacks();

  // ResultCode request(const char * contextId, const DigitalInputVariantVector& inputs);
  ResultCode request(const char * contextId, const GpioInputLinesVector& inputs);

  void release()override;

  // int debounce(LineStateMap& changes) override;

  int getLineValue(uint32_t line);

protected:
  void initialiseLineStates(const GpioInputLinesVector& inputs);

  bool updateLineStates(int* numEvents);
  bool continueDebouncing(int* numDebounced);
  bool callbackWithChangedLineStates();

  static uint64_t getCurrentTime();

private:
  GpioEventQueue m_eventQueue;
  Gpio& m_gpio;
  LineStateVector m_lineStates;
  optional<reference_wrapper<Callback>> m_pCallback;
  GpioConsumerString m_consumer;
  uint64_t m_debouncePeriod;
  Thread m_thread;
  Mutex m_callbackMutex;

  void* m_pChip;
  void* m_pLineRequest;
};



