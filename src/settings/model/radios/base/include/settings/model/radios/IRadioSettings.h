#pragma once

#include <settings/model/message/FieldUpdateSink.h>
#include "ReceiverSettings.h"
#include "TransmitterSettings.h"
#include "IActiveBandSettingsT.h"
#include <settings/model/proto/RadioPayloads.pb.h>
#include <settings/model/message/ResolveDottedStringFunc.h>
#include <settings/model/message/IMessage.h>

class IRadioSettings
{
public:
  virtual ~IRadioSettings() = default;
  [[nodiscard]] virtual bool hasActiveBands() const = 0;
  // virtual IActiveBandSettings* activeBands() = 0;
  // [[nodiscard]] virtual const IActiveBandSettings* activeBands() const = 0;
  [[nodiscard]] virtual const Band * getFocusBand() const = 0;
  [[nodiscard]] virtual const Mode * getFocusMode() const = 0;

  [[nodiscard]] virtual bool hasReceiver() const = 0;
  virtual ReceiverSettings* receiver() = 0;
  [[nodiscard]] virtual const ReceiverSettings* receiver() const = 0;

  [[nodiscard]] virtual bool hasTransmitter() const { return false; }
  virtual TransmitterSettings* transmitter() { return nullptr; }
  [[nodiscard]] virtual const TransmitterSettings* transmitter() const { return nullptr; }

  [[nodiscard]] virtual bool hasPtt() const = 0;
  [[nodiscard]] virtual bool ptt() const = 0;

#ifdef USE_DOTTED_STRING_PATHS
  virtual ResolveDottedStringFunc resolveDottedStringFunc() = 0;

#else
  virtual ResolveDottedStringFunc resolveDottedStringFunc()
  {
    return [](const char* dottedPath, FieldDescriptor& descriptor) -> ResultCode {
      return ResultCode::ERR_CONFIG_DOTTED_STRINGS_NOT_SUPPORTED;
    };
  }
#endif
};
