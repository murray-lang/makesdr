#pragma once
#include <ResultCode.h>
#include <transport/in/TransportInBaseT.h>
#include <transport/qt/event/MessageEventT.h>
#include <transport/qt/event/QtTransportBase.h>
#include <transport/qt/event/globalQtEventTargets.h>
#include <config/struct/QtTransportInConfig.h>
#include <QObject>

template<typename MessageType>
class QtTransportInT : public TransportInBaseT<MessageType>, public QObject
{
public:
  QtTransportInT()
    : m_target(Config::QtTransport::Radio)
    , m_message(Config::QtTransport::Settings)
    , m_isOpen(false)
  {
  }

  QtTransportInT(Config::QtTransport::Target role, Config::QtTransport::Message qtTarget)
    : m_target(role)
    , m_message(qtTarget)
    , m_isOpen(false)
  {
  }

  QtTransportInT(const QtTransportInT& other)
    : m_target(other.m_target)
    , m_message(other.m_message)
    , m_isOpen(false)
  {
  }

  QtTransportInT(QtTransportInT&& other) noexcept
    : m_target(other.m_target)
    , m_message(other.m_message)
    , m_isOpen(other.m_isOpen)
  {
    other.m_isOpen = false;
  }

  ~QtTransportInT() override
  {
    QtTransportInT::close();
  }

  ResultCode configure()
  {
    return configure(m_message, m_target);
  }

  ResultCode configure(const Config::QtTransportIn::Fields& config)
  {
    if (config.message == Config::QtTransport::Message::None) {
      return ResultCode::OK;
    }
    return configure(config.message, config.target);
  }

  ResultCode configure(Config::QtTransport::Message message, Config::QtTransport::Target target)
  {
    if (message == Config::QtTransport::Message::None) {
      return ResultCode::OK;
    }
    if (target == Config::QtTransport::Target::Radio) {
      switch (message) {
      case Config::QtTransport::Message::Settings:
      case Config::QtTransport::Message::Update:
        m_target = target;
        m_message = message;
        return ResultCode::OK;

      default:
        return ResultCode::ERR_CONFIG_INVALID_QT_TARGET_FOR_RADIO_INPUT;
      }
    } else {
      switch (message) {
      case Config::QtTransport::Message::Settings:
      case Config::QtTransport::Message::Modes:
      case Config::QtTransport::Message::Bands:
      case Config::QtTransport::Message::Meter:
      case Config::QtTransport::Message::Iq:
        m_target = target;
        m_message = message;
        return ResultCode::OK;
      default:
        return ResultCode::ERR_CONFIG_INVALID_QT_TARGET_FOR_CLIENT_INPUT;
      }
    }
  }

  ResultCode open() override
  {
    if (m_isOpen) {
      return ResultCode::OK;
    }
    ResultCode rc = setQtEventTarget();
    if (rc != ResultCode::OK) return rc;
    m_isOpen = true;
    return ResultCode::OK;
  }

  void close() override
  {
    if (m_isOpen) {
      QtEventTargets::setTarget(m_target, m_message, nullptr);
      m_isOpen = false;
    }
  }

  ResultCode setQtEventTarget()
  {
    return QtEventTargets::setTarget(m_target, m_message, this);
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
  Config::QtTransport::Target m_target;
  Config::QtTransport::Message m_message;
  bool m_isOpen;
};
