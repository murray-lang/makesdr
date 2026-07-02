#include "gpio/service/GpioInputLinesSource.h"
#include <gpiod.h>
#include <variant>

// #include <stm32h745i/setup/config.h>
// #include <stm32h745i/app/support/safe_printf.h>

GpioInputLinesSource::GpioInputLinesSource()
  : m_running(false)
  , m_thread(*this)

  , m_pEventBuffer(nullptr)
  , m_lineReader(GpioLineReader::create<GpioInputLinesSource, &GpioInputLinesSource::readLine>(*this))
{
}

ResultCode
GpioInputLinesSource::start()
{
  if (m_running) {
    return ResultCode::ERR_GPIO_INPUT_ALREADY_RUNNING;
  }
  m_running = true;
  if (m_thread.start()) {
    return ResultCode::OK;
  }
  return ResultCode::ERR_GPIO_INPUT_THREAD_START_FAILED;
}

void
GpioInputLinesSource::stop()
{
  m_running = false;
  m_thread.join();
}

ResultCode
GpioInputLinesSource::readLine(GpioLineMask mask, GpioLineValue* valueOut)
{
  uint32_t lineNo = __builtin_ctz(mask);
  int value = gpiod_line_request_get_value(static_cast<gpiod_line_request *>(m_pLineRequest), lineNo);
  if (value < 0) {
    return ResultCode::ERR_GPIO_INPUT_LINE_READ_FAILED;
  }
  *valueOut = static_cast<GpioLineValue>(value);
  return ResultCode::OK;
}

void
GpioInputLinesSource::callback(GpioLineEvent& event)
{
  for (auto& callbackItem : m_lineEventCallbacks) {
    if (callbackItem.mask & event.mask) {
      (*callbackItem.callback)(&event);
    }
  }
}

ResultCode
GpioInputLinesSource::requestLines(const char* context, GpioInputLinesRequestVector& requests)
{
  gpiod_line_config *lcfg = gpiod_line_config_new();
  if (lcfg == nullptr) {
    return ResultCode::ERR_SETTING_DIGITAL_INPUT_LINE_CONFIG;
  }
  for (const auto& request : requests) {
    gpiod_line_settings *ls = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(ls, GPIOD_LINE_DIRECTION_INPUT);
    gpiod_line_settings_set_bias(ls, static_cast<gpiod_line_bias>(request->config.getBias()));
    gpiod_line_settings_set_edge_detection(ls, static_cast<gpiod_line_edge>(request->config.getEdge()));

    GpioLineNumberVector lineNos; // = input.getLines();
    GpioLineMask requestedLineMask = request->config.getLines();
    getLineNumbers(requestedLineMask, lineNos);

    gpiod_line_config_add_line_settings(lcfg, lineNos.data(), lineNos.size(), ls);
    gpiod_line_settings_free(ls);

    request->lineReader = m_lineReader;
    m_lineEventCallbacks.push_back({requestedLineMask, &request->lineEventCallback});
    m_transitionHandlers.addTransitionHandler(requestedLineMask, request);
  }

  gpiod_request_config *rcfg = gpiod_request_config_new();
  gpiod_request_config_set_consumer(rcfg, context);

  gpiod_line_request *pLineRequest = gpiod_chip_request_lines(static_cast<gpiod_chip*>(m_pChip), rcfg, lcfg);
  gpiod_request_config_free(rcfg);
  gpiod_line_config_free(lcfg);
  if (pLineRequest == nullptr) {
    return ResultCode::ERR_SETTING_DIGITAL_INPUT_LINE_REQUEST_FAILED;
  }
  m_pLineRequest = pLineRequest;
  // m_eventQueue.setLineRequest(pLineRequest);
  // initialiseLineStates(inputs);
  return ResultCode::OK;
}


void
GpioInputLinesSource::handlePinTransition(GpioLineMask mask, Timestamp timestamp)
{
  if (mask == 0) return;

  GpioLineEvent event{};
  bool invokeCallbacks = true;
  GpioLineTransitionHandler* handler = m_transitionHandlers.getTransitionHandler(mask);
  if (handler != nullptr) {
    // if (mask == Digital_Input_4_Pin) BSP_LED_Toggle(LED_RED);
    invokeCallbacks = handler->handleLineTransition(mask, timestamp, &event);
  } else {
    // uint16_t lineNo = __builtin_ctz(mask);
    event.changed = true;
    event.mask = mask;
    event.timestamp = timestamp;
    gpioReadLine(mask, &event.value); // Just send the raw value
  }
  if (invokeCallbacks) {
    callback(event);
  }
}

int
GpioInputLinesSource::waitEdgeEvents(int64_t timeout_ns) const
{
  return gpiod_line_request_wait_edge_events(m_pLineRequest, timeout_ns);
}

int
GpioInputLinesSource::readEdgeEvents(struct gpiod_edge_event_buffer* buf, size_t max_events) const
{
  return gpiod_line_request_read_edge_events(m_pLineRequest, buf, max_events);
}

bool
GpioInputLinesSource::handleEdgeEvents()
{
  int numEvents = readEdgeEvents(m_pEventBuffer, 64);
  if (numEvents < 0) {
    return false;
  }
  for (int i = 0; i < numEvents; ++i) {
    gpiod_edge_event* ev = gpiod_edge_event_buffer_get_event(m_pEventBuffer, i);
    uint32_t line = gpiod_edge_event_get_line_offset(ev);
    // gpiod_edge_event_type type = gpiod_edge_event_get_event_type(ev);
    uint64_t timestamp = gpiod_edge_event_get_timestamp_ns(ev);

    handlePinTransition(1 << line, timestamp);
  }
  return true;
}

void
GpioInputLinesSource::run()
{
  constexpr int64_t idleTimeout = 100'000'000;
  while (m_running) {
    int wr = waitEdgeEvents(idleTimeout);
    if (!m_running) break;
    if (wr < 0) {
      // error!
      continue;
    }
    if (wr > 0) {
      // qDebug() <<"--------------";
      handleEdgeEvents();
    } else {
      continueDebouncing();
    }
  }
}

void
GpioInputLinesSource::continueDebouncing()
{
  auto& handlers = m_transitionHandlers.getHandlers();
  for (auto& handler : handlers) {
    auto* debouncer = get_if<GpioLineDebouncer>(&handler);
    if (debouncer != nullptr) {
      if (debouncer->isDebouncedNow()) {
        debouncer->resetDebouncing();
        GpioLineEvent event{};
        debouncer->getEvent(&event);
        callback(event);
      }
    }
  }
}
