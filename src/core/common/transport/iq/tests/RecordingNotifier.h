#pragma once

#include <settings/model/message/PayloadType.h>
#include <settings/model/radio/iq/BasicIqRxSettings.h>
#include <transport/IqMessageRingsT.h>

#include <vector>

// Stands in for a platform's consumer: records the payload type of every wake
// it is sent.
struct WakeRecorder
{
  std::vector<PayloadType> wakes;
};

// A synchronous stand-in for a platform notifier such as QtNotifier: while a
// recorder is attached, every notification is appended to it; while none is,
// notifications are dropped.
class RecordingNotifier
{
public:
  void attach(WakeRecorder* target) { m_target = target; }
  void detach()                     { m_target = nullptr; }

  void operator()(PayloadType payloadType)
  {
    if (m_target != nullptr) {
      m_target->wakes.push_back(payloadType);
    }
  }

private:
  WakeRecorder* m_target = nullptr;
};

using TestRings = IqMessageRingsT<BasicIqRxSettings, RecordingNotifier>;

inline PayloadType settingsPayloadType()
{
  return static_cast<PayloadType>(BasicIqRxSettings::payloadType);
}

// Commits an empty message straight into a pipe, bypassing either interface.
template<typename PipeT>
bool publish(PipeT& pipe)
{
  auto* pmsg = pipe.reserveWrite();
  if (pmsg == nullptr) {
    return false;
  }
  pipe.commitWrite(pmsg);
  return true;
}
