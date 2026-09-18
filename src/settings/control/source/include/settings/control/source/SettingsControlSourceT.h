#pragma once
#include <settings/control/SettingsControlBase.h>
#include <settings/model/radios/RadioSettingsSourceT.h>
#include <settings/model/radios/RadioSettingsSinkT.h>
#include <settings/model/message/FieldUpdateSource.h>
#include <settings/model/message/FieldUpdateSink.h>


template <typename RadioSettingsT>
class SettingsControlSourceT :
  public SettingsControlBase,
  public RadioSettingsSourceT<RadioSettingsT>,
  public FieldUpdateSource
{
public:
  SettingsControlSourceT()
    : SettingsControlBase()
    , m_pSettingsSink(nullptr)
    , m_pFieldUpdateSink(nullptr)
  {
  }
  SettingsControlSourceT(SettingsControlSourceT&& rhs) noexcept = default;
  ~SettingsControlSourceT() override = default;
  SettingsControlSourceT& operator=(SettingsControlSourceT&& rhs) noexcept = default;

  void connectRadioSettingsSink(RadioSettingsSinkT<RadioSettingsT>* sink) override
  {
    m_pSettingsSink = sink;
  }

  void connectFieldUpdateSink(FieldUpdateSink* sink) override
  {
    m_pFieldUpdateSink =  sink;
  }
protected:
  ResultCode notifySettings(RadioSettingsT& radioSettings) override
  {
    if (m_pSettingsSink) {
      return m_pSettingsSink->applySettings(radioSettings);
    }
    return ResultCode::OK;
  }


  ResultCode notifyFieldUpdate(const FieldUpdate& update) override
  {
    if (m_pFieldUpdateSink) {
      m_pFieldUpdateSink->applyFieldUpdate(update);
    }
    return ResultCode::OK;
  }

protected:
  RadioSettingsSinkT<RadioSettingsT>* m_pSettingsSink;
  FieldUpdateSink* m_pFieldUpdateSink;
};
