#pragma once
#include <QObject>

// Base class to provide QObject functionality for QtSettingsControlSinkT template
// This allows the template to derive from QObject without needing Q_OBJECT itself
class QtTransportBase : public QObject
{
  Q_OBJECT
public:
  QtTransportBase() = default;
  ~QtTransportBase() override = default;

// protected:
  // QObject cannot be moved or copied
  QtTransportBase(const QtTransportBase&) = delete;
  QtTransportBase& operator=(const QtTransportBase&) = delete;
  QtTransportBase(QtTransportBase&&) = delete;
  QtTransportBase& operator=(QtTransportBase&&) = delete;
};