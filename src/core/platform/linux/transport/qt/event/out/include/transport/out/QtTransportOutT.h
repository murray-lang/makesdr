#pragma once
#include <settings/model/message/MessageSinkT.h>
#include <config/struct/QtTransportOutConfig.h>
#include <transport/out/TransportOutBaseT.h>
#include <transport/qt/event/QtTransportBase.h>
#include <transport/qt/event/globalQtEventTargets.h>
#include <transport/qt/event/MessageEventT.h>
#include <settings/model/message/PayloadSource.h>
#include <QCoreApplication>

template<typename MessageType, PayloadSource source>
class QtTransportOutT : public QtTransportBase, public TransportOutBaseT<MessageType>
{
public:
  QtTransportOutT()
    : m_target(Config::QtTransport::Radio)
    , m_message(Config::QtTransport::Settings)
    , m_qtEventTarget(nullptr)
  {
  }

  QtTransportOutT(Config::QtTransport::Target role, Config::QtTransport::Message qtTarget)
    : m_target(role)
    , m_message(qtTarget)
    , m_qtEventTarget(nullptr)
  {
  }

  QtTransportOutT(const QtTransportOutT& other)
    : m_target(other.m_target)
    , m_message(other.m_message)
    , m_qtEventTarget(other.m_qtEventTarget)
  {}

  QtTransportOutT(QtTransportOutT&& other) noexcept
    : m_target(other.m_target)
    , m_message(other.m_message)
    , m_qtEventTarget(other.m_qtEventTarget)
  {}

  ResultCode configure()
  {
    return configure(m_message, m_target);
  }

  ResultCode configure(const Config::QtTransportOut::Fields& config)
  {
    if (config.message == Config::QtTransport::Message::None) {
      return ResultCode::OK;
    }
    return configure(config.message, config.target);
  }


  ResultCode configure(Config::QtTransport::Message message, Config::QtTransport::Target target)
  {
    if (target == Config::QtTransport::Radio) {
      switch (message) {
        case Config::QtTransport::Message::Settings:
        case Config::QtTransport::Message::Update:
          m_target = target;
          m_message = message;
          return ResultCode::OK;
        default:
          return ResultCode::ERR_CONFIG_INVALID_QT_TARGET_FOR_CLIENT_OUTPUT;
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
        return ResultCode::ERR_CONFIG_INVALID_QT_TARGET_FOR_RADIO_OUTPUT;
      }
    }

  }

  ResultCode open() override
  {
    return getQtEventTarget();
  }

  bool isOpen() const
  {
    return m_qtEventTarget != nullptr;
  }

  ResultCode getQtEventTarget()
  {
    m_qtEventTarget = QtEventTargets::getTarget(m_target, m_message);
    return m_qtEventTarget == nullptr ? ResultCode::ERR_CONFIG_INVALID_QT_TARGET_FOR_CLIENT_OUTPUT : ResultCode::OK;
  }

  void close() override
  {
     m_qtEventTarget = nullptr;
  }

  ResultCode applyMessage(MessageType* message) override
  {
    if (m_qtEventTarget != nullptr && message != nullptr) {
      message->setSource(source);
      auto* event = new MessageEventT<MessageType>(*message);

      QCoreApplication::postEvent(m_qtEventTarget, event);
    }

    return ResultCode::OK;
  }

protected:
  Config::QtTransport::Target m_target;
  Config::QtTransport::Message m_message;
  QObject* m_qtEventTarget;
};