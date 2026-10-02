#pragma once

template<typename T>
class IMessagePublisherT
{
public:
  virtual ~IMessagePublisherT() = default;
  [[nodiscard]] virtual T* reserve() = 0;
  virtual void commit(T* pmsg) = 0;      // publishes AND notifies
};