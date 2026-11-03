#pragma once

#include <cstdint>

#include <settings/model/message/PayloadType.h>

//*****************************************************************************
// Payload types as bits of a 32-bit mask, for notifiers that wake a consumer
// by setting bits (e.g. FreeRTOS task notifications) rather than by posting
// the payload type itself as Qt does. Each payload type's bit is its value.
//*****************************************************************************
namespace PayloadTypeBits
{
  static_assert(_makesdr_RadioPayloadType_MAX < 32,
                "Payload type values must stay below 32 to fit a 32-bit mask");

  constexpr uint32_t maskOf(PayloadType payloadType)
  {
    return uint32_t{1} << payloadType;
  }

  // Calls f(payloadType) for each payload type whose bit is set in mask,
  // lowest first.
  template<typename FuncT>
  void forEach(uint32_t mask, FuncT&& f)
  {
    while (mask != 0) {
      const auto bit = static_cast<unsigned>(__builtin_ctz(mask));
      mask &= mask - 1;
      f(static_cast<PayloadType>(bit));
    }
  }
}
