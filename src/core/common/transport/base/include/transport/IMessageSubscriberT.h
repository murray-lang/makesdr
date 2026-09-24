#pragma once

template<typename T>
class IMessageSubscriberT
{
public:
  virtual ~IMessageSubscriberT() = default;
  [[nodiscard]] virtual T* reserve() = 0;
  virtual void commit(T* pmsg) = 0;      // publishes AND notifies
};