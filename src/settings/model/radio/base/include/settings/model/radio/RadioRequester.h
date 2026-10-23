#pragma once
#include <ResultCode.h>

#include <settings/model/update/FieldUpdateSource.h>

class RadioRequester : public FieldUpdateSource
{
public:

  enum Requests
  {
    REQUEST_NONE,
    REQUEST_CAPABILITIES,
    REQUEST_SETTINGS,
    REQUEST_MODES,
    REQUEST_BANDS
  };

  RadioRequester() = default;
  RadioRequester(FieldUpdateSink* sink) { RadioRequester::connectFieldUpdateSink(sink); }
  ~RadioRequester() override = default;


  ResultCode requestCapabilities()
  {
    FieldDescriptor descriptor({ 0, Requests::REQUEST_CAPABILITIES});
    return notifyFieldUpdate(FieldUpdate(descriptor, true, FieldUpdateMeaning::VALUE));
  }

  ResultCode requestSettings()
  {
    FieldDescriptor descriptor({ 0, Requests::REQUEST_SETTINGS});
    return notifyFieldUpdate(FieldUpdate(descriptor, true, FieldUpdateMeaning::VALUE));
  }

  ResultCode requestModes()
  {
    FieldDescriptor descriptor({ 0, Requests::REQUEST_MODES});
    return notifyFieldUpdate(FieldUpdate(descriptor, true, FieldUpdateMeaning::VALUE));
  }
  ResultCode requestBands()
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
