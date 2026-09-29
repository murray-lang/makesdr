#pragma once
#include <settings/control/SettingsControlBase.h>
#include <settings/model/radio/RadioSettingsSourceT.h>
#include <settings/model/radio/RadioSettingsSinkT.h>
#include <settings/model/update/FieldUpdateSource.h>
#include <settings/model/update/FieldUpdateSink.h>


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
