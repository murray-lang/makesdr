#pragma once
#include <transport/SpectrumPublisher.h>
#include <transport/IqPublisher.h>
#include <transport/RxMeteringPublisher.h>

class IRadioPublishers
{
public:
  virtual ~IRadioPublishers() = default;

  virtual IqPublisher* getIqPublisher() = 0;
  virtual RxMeteringPublisher* getRxMeteringPublisher() = 0;
  virtual SpectrumPublisher* getSpectrumPublisher() = 0;
};
