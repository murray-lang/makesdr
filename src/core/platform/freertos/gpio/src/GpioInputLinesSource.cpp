#include "gpio/service/GpioInputLinesSource.h"

#include <stm32h745i/setup/config.h>
#include <stm32h745i/app/support/safe_printf.h>

GpioInputLinesSource::GpioInputLinesSource()
  : m_running(false)
  , m_thread(*this)
{
}

ResultCode
GpioInputLinesSource::start(uint32_t stackSize, uint32_t priority, const char* name)
{
  if (m_running) {
    return ResultCode::ERR_GPIO_INPUT_ALREADY_RUNNING;
  }
  m_running = true;
  if (m_thread.start(stackSize, priority, name)) {
    return ResultCode::OK;
  }
  return ResultCode::ERR_GPIO_INPUT_THREAD_START_FAILED;
}

void
GpioInputLinesSource::stop()
{
  m_running = false;
  // Send dummy event to unblock the waiting task
  GpioLineEvent dummyEvent = {0, false, 0, 0};
  m_eventQueue.send(dummyEvent, 0);
  m_thread.join();
}

ResultCode
GpioInputLinesSource::requestLines(const char* context, GpioInputLinesRequest* request)
{
  GpioLineMask requestedLineMask = request->config.getLines();
  bool is4or8 = (requestedLineMask & Digital_Input_4_Pin) || (requestedLineMask & Digital_Input_8_Pin);
  for (int i = 0; i < MAX_GPIO_LINES; i++) {
    GpioLineMask lineMask = requestedLineMask & (1 << i);

    if (lineMask == 0) continue;

    // Allow duplicate assignments for now. It could be handy for a wake-up function
    // if ((lineMask & m_assignedLines) != 0) {
    //   return ResultCode::ERR_GPIO_INPUT_LINE_ALREADY_ASSIGNED;
    // }
    // m_assignedLines |= lineMask;
    if (!isGpioInputLineAvailable(lineMask)) {
      // return static_cast<ResultCode>(requestedLineMask);
      return ResultCode::ERR_GPIO_INPUT_LINE_NOT_AVAILABLE;
    }
  }
  request->lineReader = gpioReadLine;
  m_lineEventCallbacks.push_back({requestedLineMask, &request->lineEventCallback});
  GpioLineTransitionHandler* handler = m_transitionHandlers.addTransitionHandler(requestedLineMask, request);
  // if (is4or8) {
  //   if (handler != nullptr) {
  //     BSP_LED_On(LED_GREEN);
  //   } else {
  //     BSP_LED_On(LED_RED);
  //   }
  // }
  return ResultCode::OK;
}

ResultCode
GpioInputLinesSource::requestLines(const char* context, GpioInputLinesRequestVector& requests)
{
  for (auto& request : requests) {
    ResultCode rc = requestLines(context, request);
    if (rc != ResultCode::OK) return rc;
  }
  return ResultCode::OK;
}


// ResultCode
// GpioInputLinesSource::readLine(GpioLineMask mask, GpioLineValue* result)
// {
//   uint16_t lineNo = __builtin_ctz(mask);
//   if (lineNo >= MAX_GPIO_LINES) {
//     return ResultCode::ERR_GPIO_INPUT_LINE_NOT_AVAILABLE;
//   }
//   *result = lineReaders[lineNo]->readLine();
//   return ResultCode::OK;
// }



void
GpioInputLinesSource::handlePinTransition(uint16_t mask, uint32_t timestamp)
{
  // if (mask == Digital_Input_4_Pin) BSP_LED_Toggle(LED_GREEN);
  // if (mask == Digital_Input_8_Pin) BSP_LED_Toggle(LED_RED);
  GpioLineEvent event{};
  GpioLineTransitionHandler* handler = m_transitionHandlers.getTransitionHandler(mask);
  if (handler != nullptr) {
    // if (mask == Digital_Input_4_Pin) BSP_LED_Toggle(LED_RED);
    if (handler->handleLineTransition(mask, timestamp, &event)) {

      enqueueEvent(event);
    }
  } else {
    // uint16_t lineNo = __builtin_ctz(mask);
    event.changed = true;
    event.mask = mask;
    event.timestamp = timestamp;
    gpioReadLine(mask, &event.value); // Just send the raw value
    enqueueEvent(event);
  }

}

void
GpioInputLinesSource::enqueueEvent(const GpioLineEvent& event)
{
  if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
    return;
  }
  // Send event to queue from ISR
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  m_eventQueue.sendFromISR(event, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void
GpioInputLinesSource::run()
{
  GpioLineEvent event{};
  while (m_running) {
    // Wait for event from queue (blocks until event available)
    if (m_eventQueue.receive(event)) {
      if (!m_running) {
        break;  // Dummy event sent by stop()
      }

      // Process the event
      if (event.mask != 0) {  // Skip dummy events
        for (auto& callbackItem : m_lineEventCallbacks) {
          if (callbackItem.mask & event.mask) {
            (*callbackItem.callback)(&event);
          }
        }
      }
    }
  }
}

void
GpioInputLinesSource::continueDebouncing()
{
  auto& handlers = m_transitionHandlers.getHandlers();
  for (auto& handler : handlers) {
    GpioLineDebouncer* debouncer = etl::get_if<GpioLineDebouncer>(&handler);
    if (debouncer != nullptr) {
      if (debouncer->isDebouncedNow()) {
        debouncer->resetDebouncing();
        GpioLineEvent event{};
        debouncer->getEvent(&event);
        m_eventQueue.send(event, 0);
      }
    }
  }
}
