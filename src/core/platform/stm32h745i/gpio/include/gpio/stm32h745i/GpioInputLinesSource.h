#ifndef STM32H745I_GPIOINPUTLINESOURCE_H_
#define STM32H745I_GPIOINPUTLINESOURCE_H_

#include <ResultCode.h>
#include <gpio/base/GpioLineEvent.h>

#include "gpio/base/GpioInputLinesRequest.h"
#include "gpio/handlers/GpioLineTransitionHandlerVariant.h"
#include "gpio/handlers/GpioTransitionHandlers.h"
#include "thread/Thread.h"
#include "thread/Runnable.h"
#include <thread/Queue.h>
#include <thread/Semaphore.h>

#ifdef USE_ETL
#include <etl/vector.h>
#else
#include <vector>
#endif

#ifdef USE_ETL
using LineEventCallbackVector = etl::vector<GpioLineEventCallback*, MAX_DIGITAL_INPUT_HANDLERS>;
using GpioLineTransitionHandlerVariantVector = etl::vector<GpioLineTransitionHandlerVariant, MAX_GPIO_LINES>;
#else
using LineEventCallbackVector = std::vector<GpioLineEventCallback*>;
using GpioLineTransitionHandlerVariantVector = std::vector<GpioLineTransitionHandlerVariant>;
#endif


#define LINE_EVENT_QUEUE_SIZE 4

class GpioLineTransitionHandler;

using GpioLineEventQueue = Queue<GpioLineEvent, LINE_EVENT_QUEUE_SIZE>;

class GpioInputLinesSource : public Runnable
{
public:
  GpioInputLinesSource(GpioLineReaderDelegate& lineReader);
  ~GpioInputLinesSource() override = default;

  ResultCode start(uint32_t stackSize, uint32_t priority, const char* name);
  void stop();

  ResultCode requestLines(GpioInputLinesRequest& request);
  ResultCode requestLines(GpioInputLinesRequestVector& requests);

  void handlePinTransition(GpioLineMask mask, uint32_t timestamp);

  void run() override;

protected:

  ResultCode readLine(GpioLineMask mask, GpioLineValue* result);

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
#endif // STM32H745I_GPIOINPUTLINESOURCE_H_