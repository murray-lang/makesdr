#pragma once
#include <ResultCode.h>
#include <transport/in/TransportInBaseT.h>
#include <transport/qt/event/MessageEventT.h>
#include <transport/qt/event/QtTransportBase.h>
#include <transport/qt/event/globalQtEventTargets.h>
#include <config/struct/QtTransportInConfig.h>
#include <QThread>
#include <QEventLoop>


template<typename MessageType>
class QtTransportInT : public TransportInBaseT<MessageType>, public QThread
{
public:
  QtTransportInT()
    : m_target(Config::QtTransport::Radio)
    , m_message(Config::QtTransport::Settings)
  {
  }

  QtTransportInT(Config::QtTransport::Target role, Config::QtTransport::Message qtTarget)
    : m_target(role)
    , m_message(qtTarget)
  {
  }

  QtTransportInT(const QtTransportInT& other)
    : m_target(other.m_role)
    , m_message(other.m_message)
  {
  }

  QtTransportInT(QtTransportInT&& other) noexcept
    : m_target(other.m_role)
    , m_message(other.m_message)
  {
  }

  ~QtTransportInT() override
  {
    QtTransportInT::close();
  }

  ResultCode configure(const Config::QtTransportIn::Fields& config)
  {
    if (config.target == Config::QtTransport::Radio) {
      if (config.message != Config::QtTransport::Settings) {
        return ResultCode::ERR_CONFIG_INVALID_QT_TARGET_FOR_RADIO_INPUT;
      }
    }
    m_target = config.target;
    m_message = config.message;
    return ResultCode::OK;
  }

  ResultCode open() override
  {
    ResultCode rc = setTarget(this);
    if (rc != ResultCode::OK) return rc;
    start();
    return ResultCode::OK;
  }

  void close() override
  {
    setTarget(nullptr);
    // emit finished();
    wait();
  }

  void run() override
  {
    QEventLoop loop;
    // QObject::connect(this, &QtTransportIn::finished, &loop, &QEventLoop::quit);
    loop.exec();
  }

  void customEvent(QEvent* event) override
  {
    if (event->type() == static_cast<QEvent::Type>(QEvent::User + MessageType::payloadType)) {

      auto* messageEvent = dynamic_cast<MessageEventT<MessageType>*>(event);
      if (messageEvent) {
        MessageType message(messageEvent->payload);
        this->notifyMessage(&message);
      }
    }
  }

protected:
  ResultCode setTarget(QObject* pTarget) const
  {
    if (m_target == Config::QtTransport::Radio) {
      if (m_message != Config::QtTransport::Settings) {
        return ResultCode::ERR_CONFIG_INVALID_QT_TARGET_FOR_RADIO_INPUT;
      }
      QtEventTargets::setTarget(m_message, pTarget);
    } else if (m_message == Config::QtTransport::Settings) {
      QtEventTargets::setTarget(static_cast<Config::QtTransport::Message>(m_message + 1), pTarget);
    } else {
      QtEventTargets::setTarget(m_message, pTarget);
    }
    return ResultCode::OK;
  }
protected:
  Config::QtTransport::Target m_target;
  Config::QtTransport::Message m_message;
};