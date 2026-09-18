#include "../include/transport/qt/event/globalQtEventTargets.h"

QObject* QtEventTargets::radioTargets[NUM_QT_RADIO_TARGETS] = { nullptr, nullptr };
QObject* QtEventTargets::clientTargets[NUM_QT_CLIENT_TARGETS] = { nullptr, nullptr, nullptr, nullptr, nullptr };

QObject*
QtEventTargets::getTarget(Config::QtTransport::Target target, Config::QtTransport::Message message)
{
  if (target == Config::QtTransport::Target::Radio) {
    if (message == Config::QtTransport::Settings) {
      return radioTargets[0];
    }
    if (message == Config::QtTransport::Update) {
      return radioTargets[1];
    }

  } else {
    if (message == Config::QtTransport::Settings) {
      return clientTargets[0];
    }
    if (message == Config::QtTransport::Modes) {
      return clientTargets[1];
    }
    if (message == Config::QtTransport::Bands) {
      return clientTargets[2];
    }
    if (message == Config::QtTransport::Meter) {
      return clientTargets[3];
    }
    if (message == Config::QtTransport::Iq) {
      return clientTargets[4];
    }
  }
  return nullptr;
}

ResultCode
QtEventTargets::setTarget(Config::QtTransport::Target target, Config::QtTransport::Message message, QObject* pTarget)
{
  if (target == Config::QtTransport::Target::Radio) {
    if (message == Config::QtTransport::Settings) {
      radioTargets[0] = pTarget;
    } else if (message == Config::QtTransport::Update) {
      radioTargets[1] = pTarget;
    } else {
      return ResultCode::ERR_QT_INVALID_TARGET;
    }
  } else {
    if (message == Config::QtTransport::Settings) {
      clientTargets[0] = pTarget;
    } else if (message == Config::QtTransport::Modes) {
      clientTargets[1] = pTarget;
    } else if (message == Config::QtTransport::Bands) {
      clientTargets[2] = pTarget;
    } else if (message == Config::QtTransport::Meter) {
      clientTargets[3] = pTarget;
    } else if (message == Config::QtTransport::Iq) {
      clientTargets[4] = pTarget;
    } else {
      return ResultCode::ERR_QT_INVALID_TARGET;
    }
  }
  return ResultCode::OK;
}

// QtEventTargets globalQtEventTargets;
