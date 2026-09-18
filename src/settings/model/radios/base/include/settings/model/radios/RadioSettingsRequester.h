#pragma once
#include <ResultCode.h>
#include "IRadioSettingsRequester.h"

#include <settings/model/message/FieldUpdateSource.h>

class RadioSettingsRequester : public IRadioSettingsRequester, public FieldUpdateSource
{
public:
  RadioSettingsRequester() = default;
  RadioSettingsRequester(FieldUpdateSink* sink) { RadioSettingsRequester::connectFieldUpdateSink(sink); }
  ~RadioSettingsRequester() override = default;


  ResultCode requestSettings() override
  {
    FieldDescriptor descriptor({ 0, Requests::REQUEST_SETTINGS});
    return notifyFieldUpdate(FieldUpdate(descriptor, true, FieldUpdateMeaning::VALUE));
  }

  ResultCode requestModes() override
  {
    FieldDescriptor descriptor({ 0, Requests::REQUEST_MODES});
    return notifyFieldUpdate(FieldUpdate(descriptor, true, FieldUpdateMeaning::VALUE));
  }
  ResultCode requestBands() override
  {
    FieldDescriptor descriptor({ 0, Requests::REQUEST_BANDS});
    return notifyFieldUpdate(FieldUpdate(descriptor, true, FieldUpdateMeaning::VALUE));
  }

  void connectFieldUpdateSink(FieldUpdateSink* sink) override
  {
    m_pSink = sink;
  }

protected:

  ResultCode notifyFieldUpdate(const FieldUpdate& settingUpdate) override
  {
    if (m_pSink) {
      return m_pSink->applyFieldUpdate(settingUpdate);
    }
    return ResultCode::OK;
  }


  FieldUpdateSink* m_pSink{nullptr};
};
