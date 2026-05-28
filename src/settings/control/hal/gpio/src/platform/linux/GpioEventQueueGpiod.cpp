#include "gpio/base/GpioEventQueue.h"
#include <gpiod.h>

GpioEventQueue::GpioEventQueue(void* pRequest, uint32_t capacity)
  : m_pLineRequest(pRequest), m_pEventBuffer(nullptr)
{
  m_pEventBuffer = gpiod_edge_event_buffer_new(MAX_GPIO_EVENT_QUEUE_CAPACITY);
}

GpioEventQueue::~GpioEventQueue()
{
  gpiod_edge_event_buffer_free(static_cast<gpiod_edge_event_buffer *>(m_pEventBuffer));
}

GpioEventQueue::GpioEventQueue(GpioEventQueue&& other) noexcept
  : m_pLineRequest(other.m_pLineRequest), m_pEventBuffer(other.m_pEventBuffer)
{
  other.m_pLineRequest = nullptr;
  other.m_pEventBuffer = nullptr;
}

GpioEventQueue&
GpioEventQueue::operator=(GpioEventQueue&& other) noexcept
{
  if (this != &other) {
    gpiod_edge_event_buffer_free(static_cast<gpiod_edge_event_buffer *>(m_pEventBuffer));

    m_pLineRequest = other.m_pLineRequest;
    m_pEventBuffer = other.m_pEventBuffer;

    other.m_pLineRequest = nullptr;
    other.m_pEventBuffer = nullptr;
  }
  return *this;
}

int32_t
GpioEventQueue::waitEdgeEvents(int64_t timeoutNs)
{
  return gpiod_line_request_wait_edge_events(static_cast<gpiod_line_request *>(m_pLineRequest), timeoutNs);
}

int32_t
GpioEventQueue::readEdgeEvents(uint32_t maxEvents)
{
  return gpiod_line_request_read_edge_events(
    static_cast< gpiod_line_request *>(m_pLineRequest),
    static_cast<gpiod_edge_event_buffer *>(m_pEventBuffer),
    maxEvents
  );
}

bool
GpioEventQueue::getEventAt(uint32_t i, GpioEvent& eventOut) const
{
  gpiod_edge_event* ev = gpiod_edge_event_buffer_get_event(static_cast<gpiod_edge_event_buffer *>(m_pEventBuffer), i);
  if (ev == nullptr) {
    return false;
  }
  eventOut.type = static_cast<GpioEvent::Type>(gpiod_edge_event_get_event_type(ev));
  eventOut.line = gpiod_edge_event_get_line_offset(ev);
  eventOut.timestamp = gpiod_edge_event_get_timestamp_ns(ev);
  return true;
}
