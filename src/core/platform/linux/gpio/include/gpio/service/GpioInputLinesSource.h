#ifndef LINUX_GPIOINPUTLINESOURCE_H_
#define LINUX_GPIOINPUTLINESOURCE_H_

#include <ResultCode.h>
#include <gpio/input/GpioLineEvent.h>

#include "gpio/input/GpioInputLinesRequest.h"
#include "gpio/input/handlers/GpioLineTransitionHandlerVariant.h"
#include "gpio/input/handlers/GpioTransitionHandlers.h"

#include <thread/Thread.h>
#include <thread/Runnable.h>
#include <thread/Queue.h>
#include <thread/Semaphore.h>

#include "GpioLinesSourceBase.h"

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

struct gpiod_line_request;
struct gpiod_edge_event_buffer;

class GpioLineTransitionHandler;

using GpioLineEventQueue = Queue<GpioLineEvent, LINE_EVENT_QUEUE_SIZE>;

class GpioInputLinesSource : public GpioLinesSourceBase, public Runnable
{
public:
  GpioInputLinesSource();
  ~GpioInputLinesSource() override = default;

  ResultCode start();
  void stop();

  // ResultCode requestLines(const char* context, GpioInputLinesRequest* request);
  ResultCode requestLines(const char* context, GpioInputLinesRequestVector& requests);

  void handlePinTransition(GpioLineMask mask, Timestamp timestamp);

  [[nodiscard]] bool isRunning() const { return m_running; }

  void run() override;

protected:
  ResultCode readLine(GpioLineMask mask, GpioLineValue* value);
  void callback(GpioLineEvent& event);
  [[nodiscard]] int waitEdgeEvents(int64_t timeout_ns) const;
  [[nodiscard]] int readEdgeEvents(struct gpiod_edge_event_buffer* buf, size_t max_events) const;
  bool handleEdgeEvents();
  void continueDebouncing();

private:
  // Below commented out to allow duplicate assignments for now. This could be handy for a wake-up function
  // GpioLineMask m_assignedLines;
  bool m_running;
  Thread m_thread;
  // GpioLineEventQueue m_eventQueue;
  // Semaphore m_semaphore;

  LineEventCallbackVector m_lineEventCallbacks;

  GpioLineTransitionHandlers m_transitionHandlers;

  gpiod_edge_event_buffer* m_pEventBuffer;
  GpioLineReader m_lineReader;
};
#endif // LINUX_GPIOINPUTLINESOURCE_H_