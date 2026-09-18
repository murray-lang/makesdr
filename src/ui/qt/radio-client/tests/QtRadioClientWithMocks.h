#pragma once
#include "QtRadioClient.h"
#include <gmock/gmock.h>

class QtRadioClientWithMocks : public QtRadioClient
{
public:
  QtRadioClientWithMocks(QObject* parent) : QtRadioClient(parent) {};
  ~QtRadioClientWithMocks() override = default;

  MOCK_METHOD(ResultCode, applySettings, (IRadioSettings& settings), (override));
  MOCK_METHOD(void, setBands, (BandCategoryList*), (override));
  MOCK_METHOD(void, setModes, (ModeList*), (override));

  // ResultCode testRequestCurrentSettings() { return requestCurrentSettings(); }
  // ResultCode testRequestModes() { return requestModes(); }
  // ResultCode testRequestBands() { return requestBands(); }
};

