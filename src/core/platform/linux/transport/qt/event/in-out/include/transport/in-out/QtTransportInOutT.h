#pragma once
#include "QtObjectBase.h"
#include "QtTransportIoConfig.h"
#include <QThread>
#include <transport/in/QtTransportInT.h>
#include <transport/out/QtTransportOutT.h>
#include <mutex>
#include <condition_variable>

using Target = Config::QtTransport::Target;
using Message = Config::QtTransport::Message;

template<typename MessageType, PayloadSource source>
class QtTransportInOutT : public QtObjectBase, public MessageSinkT<MessageType>
{
public:
  QtTransportInOutT(QThread& thread)
    : QtObjectBase()
    , m_thread(thread)
    , m_in()
    , m_out()
    , m_enableIn(false)
    , m_enableOut(false)
    , m_messageReceived(false)
  {
    m_in.moveToThread(&m_thread);
    m_in.connectMessageSink(this);
  }

  void connectMessageInSink(MessageSinkT<MessageType>* sink)
  {
    m_in.connectMessageSink(sink);
  }

  ResultCode configure(QtTransportIoConfig& config)
  {
    m_enableIn = config.in.message != Message::None;
    m_enableOut = config.out.message != Message::None;
    ResultCode result = ResultCode::OK;
    if (m_enableIn) {
      result = m_in.configure(config.in);
      if (result != ResultCode::OK) return result;
    }

    if (m_enableOut) {
      result = m_out.configure(config.out);
      if (result != ResultCode::OK) return result;
    }
    return setTargets();
  }

  ResultCode setTargets()
  {
    if (m_enableIn) {
      return m_in.setQtEventTarget();
    }
    return ResultCode::OK;
  }

  ResultCode applyMessage(MessageType* message) final
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    // m_receivedMessage.replace(*message, true);
    m_receivedMessage = *message;
    m_messageReceived = true;
    m_cv.notify_all();
    return ResultCode::OK;
  }

  ResultCode start()
  {
    ResultCode rc = ResultCode::OK;
    if (m_enableOut) {
      m_out.open(); // Allow to fail because can open later when sending
    }
    if (m_enableIn) {
      rc = m_in.open();
      // m_thread.start();
    }
    return rc;
  }

  void stop()
  {
    if (m_enableIn) {
      // m_thread.quit();
      // m_thread.wait();
      m_in.close();
    }
    if (m_enableOut) {
      m_out.close();
    }
  }

  ResultCode send(MessageType* message)
  {
    if (!m_enableOut) {
      return ResultCode::ERR_QT_TRANSPORT_OUTPUT_NOT_ENABLED;
    }
    if (!m_out.isOpen()) {
      ResultCode rc = m_out.open();
      if (rc != ResultCode::OK) return rc;
    }
    return m_out.applyMessage(message);
  }

  bool waitForMessage(std::chrono::milliseconds timeout = std::chrono::milliseconds(1000))
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_cv.wait_for(lock, timeout, [this] { return m_messageReceived; });
  }

  MessageType& getReceivedMessage()
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_receivedMessage;
  }

  void reset()
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_messageReceived = false;
  }

  [[nodiscard]] bool messageReceived() const { return m_messageReceived; }

private:
  QThread& m_thread;
  QtTransportInT<MessageType> m_in;
  QtTransportOutT<MessageType, source> m_out;
  bool m_enableIn;
  bool m_enableOut;


  std::mutex m_mutex;
  std::condition_variable m_cv;
  bool m_messageReceived = false;
  MessageType m_receivedMessage;
};