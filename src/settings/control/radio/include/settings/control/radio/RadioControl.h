#pragma once

#include <settings/model/core/RadioSettingsSink.h>
#include <settings/model/core/RadioSettingsSource.h>
#include <settings/control/sink/PttSink.h>
#include <settings/control/factory/SettingsControlSinkVariant.h>
#include <settings/control/factory/SettingsControlSourceVariant.h>

#include <config/struct/ControlConfig.h>

#ifdef USE_ETL
#include <etl/vector.h>

using ControlSinkVector = etl::vector<SettingsControlSinkVariant, MAX_CONTROL_SINKS>; //
using ControlSourceVector = etl::vector<SettingsControlSourceVariant, MAX_CONTROL_SOURCES>;
#else
#include <vector>

using ControlSinkVector = std::vector<SettingsControlSinkVariant>;
using ControlSourceVector = std::vector<SettingsControlSourceVariant>;
#endif

class RadioControl :
  public RadioSettingsSource,
  public RadioSettingsSink,
  public SettingFieldUpdateSink,
  public SettingFieldUpdateSource,
  public PttSink
{
public:
  RadioControl();
  ~RadioControl() override = default;

  ResultCode configure(const Config::Control::Fields& pConfig);
  ResultCode start();
  void stop();

  ResultCode applySettings(const RadioSettings& settings) override;
  ResultCode applySettingFieldUpdate(const SettingFieldUpdate& settingDelta) override;


  void connectSink(RadioSettingsSink& sink) override;
  void connectSettingFieldUpdateSink(SettingFieldUpdateSink& sink) override;

  ResultCode notifySettings(const RadioSettings& settings) override;
  ResultCode notifySettingFieldUpdate(const SettingFieldUpdate& settingUpdate) override;

  // PttSink Method
  void ptt(bool on) override;


protected:
  // Intercepts settings from m_controlSources for anything relevant to this RadioControl mechanism
  // One of these gets connect()ed to each control source
  // ControlSources have no concept of BandSettings, since these are internal to the radio.
  class InternalSink : public RadioSettingsSink, public SettingFieldUpdateSink
  {
  public:
    explicit InternalSink(RadioControl* pControl) : m_pControl(pControl) {}
    ResultCode applySettings(const RadioSettings& settings) override
    {
      if (m_pControl) {
        return m_pControl->notifySettings(settings); // Notify external sink
      }
      return ResultCode::OK;
    }
    ResultCode applySettingFieldUpdate(const SettingFieldUpdate& settingDelta) override
    {
      if (m_pControl) {
        return m_pControl->notifySettingFieldUpdate(settingDelta); // Notify external sink
      }
      return ResultCode::OK;
    }

  protected:
    RadioControl* m_pControl;
  };
protected:
  ControlSinkVector m_controlSinks;
  ControlSourceVector m_controlSources;
  InternalSink m_internalSink;
  optional<reference_wrapper<RadioSettingsSink>> m_externalSettingsSink;
  optional<reference_wrapper<SettingFieldUpdateSink>> m_externalFieldUpdateSink;
};