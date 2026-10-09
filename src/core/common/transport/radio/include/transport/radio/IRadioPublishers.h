#pragma once
#include "FftPublisher.h"
#include "IqPublisher.h"
#include "RxMeteringPublisher.h"

class IRadioPublishers
{
public:
  virtual ~IRadioPublishers() = default;
  virtual IqPublisher* getIqPublisher() = 0;
  virtual RxMeteringPublisher* getRxMeteringPublisher() = 0;
  virtual FftPublisher* getFftPublisher() = 0;
};
