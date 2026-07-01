#ifndef FREERTOS_GPIOINPUTLINESOURCE_H_
#define FREERTOS_GPIOINPUTLINESOURCE_H_

#include <ResultCode.h>
#include <gpio/input/GpioLineEvent.h>

#include "gpio/input/GpioInputLinesRequest.h"
#include "gpio/input/handlers/GpioLineTransitionHandlerVariant.h"
#include "gpio/input/handlers/GpioTransitionHandlers.h"
#include "thread/Thread.h"
#include "thread/Runnable.h"
#include <thread/Queue.h>
#include <thread/Semaphore.h>

#ifdef USE_ETL
#include <etl/vector.h>
#else
#include <vector>
#endif

struct LineEventCallbackItem {
  GpioLineMask mask;
  GpioLineEventCallback* callback;
};

#ifdef USE_ETL
using LineEventCallbackVector = etl::vector<LineEventCallbackItem, MAX_DIGITAL_INPUT_HANDLERS>;
using GpioLineTransitionHandlerVariantVector = etl::vector<GpioLineTransitionHandlerVariant, MAX_GPIO_LINES>;
#else
using LineEventCallbackVector = std::vector<LineEventCallbackItem>;
using GpioLineTransitionHandlerVariantVector = std::vector<GpioLineTransitionHandlerVariant>;
#endif


#define LINE_EVENT_QUEUE_SIZE 4

class GpioLineTransitionHandler;

using GpioLineEventQueue = Queue<GpioLineEvent, LINE_EVENT_QUEUE_SIZE>;

class GpioInputLinesSource : public Runnable
{
public:
  GpioInputLinesSource();
  ~GpioInputLinesSource() override = default;

  ResultCode start(uint32_t stackSize, uint32_t priority, const char* name);
  void stop();

  ResultCode requestLines(const char* context, GpioInputLinesRequest* request);
  ResultCode requestLines(const char* context, GpioInputLinesRequestVector& requests);

  void handlePinTransition(GpioLineMask mask, uint32_t timestamp);

  [[nodiscard]] bool isRunning() const { return m_running; }

  void run() override;

protected:

  // static ResultCode readLine(GpioLineMask mask, GpioLineValue* result);

  void enqueueEvent(const GpioLineEvent& event);
  void continueDebouncing();


private:
  // Allow duplicate assignments for now. It could be handy for a wake-up function
  // GpioLineMask m_assignedLines;
  bool m_running;
  Thread m_thread;
  GpioLineEventQueue m_eventQueue;
  Semaphore m_semaphore;

  LineEventCallbackVector m_lineEventCallbacks;

  GpioLineTransitionHandlers m_transitionHandlers;
};
#endif // FREERTOS_GPIOINPUTLINESOURCE_H_