#pragma once

#include <settings/control/sink/SettingsControlSinkT.h>

#ifdef IS_LINUX
#include <config/struct/QtTransportOutConfig.h>
#include <transport/out/QtTransportOutT.h>

template <typename T>
using TransportOutT = QtTransportOutT<T, PayloadSource::SOURCE_BACK_END>;
#endif

template <typename RadioSettingsT>
class TransportControlSinkT : public SettingsControlSinkT<RadioSettingsT>, public FieldUpdateSink
{
public:
  TransportControlSinkT()
    : m_transport()
  {}

  TransportControlSinkT(const TransportControlSinkT& other)
    : m_transport(other.m_transport)
  {}

  TransportControlSinkT(TransportControlSinkT&& other) noexcept
    : m_transport(move(other.m_transport))
  {}

  ~TransportControlSinkT() override = default;

#ifdef IS_LINUX
  ResultCode configure(const Config::QtTransportOut::Fields& config)
  {
    return m_transport.configure(config);
  }
#endif

  bool discover() override { return true; }
  ResultCode open() override { return m_transport.open(); }
  void close() override { m_transport.close(); }
  void exit() override {}

  ResultCode applySettings(RadioSettingsT& settings) override
  {
    return m_transport.applyMessage(&settings);
  }

  ResultCode ptt(bool on) override { return ResultCode::OK; }

  ResultCode applyFieldUpdate(const FieldUpdate& settingDelta) override
  {
    return ResultCode::OK;  //Field updates not transported at this stage.
  }

protected:
  TransportOutT<RadioSettingsT> m_transport;
};

