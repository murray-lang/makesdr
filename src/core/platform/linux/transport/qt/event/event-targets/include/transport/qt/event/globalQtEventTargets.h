#pragma once

#include <ResultCode.h>
#include <QObject>

#include <config/struct/QtTransportConfig.h>

#define NUM_QT_RADIO_TARGETS 2 // Settings, Update
#define NUM_QT_CLIENT_TARGETS 5 // Settings, Modes, Bands, Meter, Iq

class QtEventTargets
{
public:
  static QObject* getTarget(Config::QtTransport::Target target, Config::QtTransport::Message message);
  static ResultCode setTarget(Config::QtTransport::Target target, Config::QtTransport::Message message, QObject* pTarget);
protected:
  static QObject* radioTargets[NUM_QT_RADIO_TARGETS];
  static QObject* clientTargets[NUM_QT_CLIENT_TARGETS];
};

// extern QtEventTargets globalQtEventTargets;
