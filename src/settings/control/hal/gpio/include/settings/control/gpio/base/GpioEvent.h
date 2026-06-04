#pragma once
#include <cstdint>

struct GpioEvent
{
// public:
  enum class Type
  {
    NONE = 0,
    RISING,
    FALLING
  };

//   GpioEvent(Type type, uint32_t line, uint64_t timestamp);
//
//   Type type() const;
//   uint32_t line() const;
//   uint64_t timestamp() const;
//
//
// private:
  Type type;
  uint32_t line;
  uint64_t timestamp;
};
