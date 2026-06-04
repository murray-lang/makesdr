#pragma once
#include <cstdint>
#include "GpioEvent.h"

class GpioEventQueue
{
public:
  GpioEventQueue(void* pRequest, uint32_t capacity);
  virtual ~GpioEventQueue();

  GpioEventQueue(const GpioEventQueue&) = delete;
  GpioEventQueue& operator=(const GpioEventQueue&) = delete;

  GpioEventQueue(GpioEventQueue&& other) noexcept;
  GpioEventQueue& operator=(GpioEventQueue&& other) noexcept;

  void setLineRequest(void* lineRequest) { m_pLineRequest = lineRequest;}

  int32_t waitEdgeEvents(int64_t timeoutNs);
  int32_t readEdgeEvents(uint32_t maxEvents);

  bool getEventAt(uint32_t index, GpioEvent& eventOut) const;

protected:
  void* m_pLineRequest;
  void* m_pEventBuffer;
};
