#pragma once

#include <transport/out/QtTransportOutT.h>
#include <settings/model/message/FieldUpdateMessage.h>
#include <settings/model/message/FieldUpdateSink.h>

template<PayloadSource source>
class QtTransportFieldUpdateOut : public QtTransportOutT<FieldUpdateMessage, source>, public FieldUpdateSink
{
public:
  QtTransportFieldUpdateOut()
    : QtTransportOutT<FieldUpdateMessage, source>()
  {
  }

  QtTransportFieldUpdateOut(Config::QtTransport::Target role)
    : QtTransportOutT<FieldUpdateMessage, source>(role, Config::QtTransport::Message::Update)
  {
  }

  QtTransportFieldUpdateOut(const QtTransportFieldUpdateOut& other) = default;
  QtTransportFieldUpdateOut(QtTransportFieldUpdateOut&& other) noexcept = default;

  ~QtTransportFieldUpdateOut() override = default;
  ResultCode applyFieldUpdate(const FieldUpdate& update) override
  {
    FieldUpdateMessage message(update, source);
    return QtTransportOutT<FieldUpdateMessage, source>::applyMessage(&message);
  }
};