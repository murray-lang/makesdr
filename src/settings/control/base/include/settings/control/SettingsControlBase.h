#pragma once
#include "ResultCode.h"


class SettingsControlBase {
public:
  virtual ~SettingsControlBase() = default;

  virtual bool discover() = 0;
  virtual ResultCode open() = 0;
  virtual void close() = 0;
  virtual void exit() = 0;
};