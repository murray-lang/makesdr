#pragma once

#include <settings/control/source/SettingsControlSourceT.h>

#ifdef IS_LINUX
#include <config/struct/QtTransportInConfig.h>
#include <transport/in/QtTransportInT.h>

template <typename T>
using TransportInT = QtTransportInT<T>;
#endif

template <typename RadioSettingsT>
class TransportControlSourceT : public SettingsControlSourceT<RadioSettingsT>
{
public:
  TransportControlSourceT()
    : m_internalMessageSink(this)
  {
    m_transport.connectMessageSink(&m_internalMessageSink);
  }

  TransportControlSourceT(const TransportControlSourceT& other)
    : m_internalMessageSink(this)
  {
    m_transport.connectMessageSink(&m_internalMessageSink);
  }

  TransportControlSourceT(TransportControlSourceT&& other) noexcept
    : m_internalMessageSink(this)
  {
    m_transport.connectMessageSink(&m_internalMessageSink);
  }

  ~TransportControlSourceT() override = default;

#ifdef IS_LINUX
  ResultCode configure(const Config::QtTransportIn::Fields& config)
  {
    return m_transport.configure(config);
  }
#endif

  bool discover() override { return true; }
  ResultCode open() override
  {

    return m_transport.open();
  }
  void close() override { m_transport.close(); }
  void exit() override {}

protected:
  class InternalMessageSink : public MessageSinkT<RadioSettingsT>
  {
  public:
    InternalMessageSink(TransportControlSourceT* parent) : m_parent(parent) {}

    ResultCode applyMessage(RadioSettingsT* settings) override
    {
      return m_parent->notifySettings(*settings);
    }

  protected:
    TransportControlSourceT* m_parent;
  };

  TransportInT<RadioSettingsT> m_transport;
  InternalMessageSink m_internalMessageSink;
};

