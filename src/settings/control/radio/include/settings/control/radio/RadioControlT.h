#pragma once

#include <CrossPlatformTypes.h>
#include <settings/model/radios/RadioSettingsSinkT.h>
#include <settings/model/radios/RadioSettingsSourceT.h>
#include <settings/model/message/FieldUpdateSource.h>
#include <settings/model/message/ResolveDottedStringFunc.h>

#include <settings/control/sink/PttSink.h>
#include <settings/control/factory/SettingsControlSinkFactoryT.h>
#include <settings/control/factory/SettingsControlSourceFactoryT.h>
#include <settings/control/factory/SettingsControlSinkTypesT.h>
#include <settings/control/factory/SettingsControlSourceTypesT.h>

#include <config/struct/ControlConfig.h>

#include <memory>

#ifdef USE_ETL
#include <etl/vector.h>
#else
#include <vector>
#endif


template <typename RadioSettingsT>
class RadioControlT :
  public RadioSettingsSourceT<RadioSettingsT>,
  public RadioSettingsSinkT<RadioSettingsT>,
  public FieldUpdateSink,
  public FieldUpdateSource,
  public PttSink
{
public:

  using SettingsType = RadioSettingsT;

#ifdef USE_ETL

  using ControlSinkVector = etl::vector<typename SettingsControlSinkTypesT<RadioSettingsT>::Variant, MAX_CONTROL_SINKS>; //
  using ControlSourceVector = etl::vector<typename SettingsControlSourceTypesT<RadioSettingsT>::Variant, MAX_CONTROL_SOURCES>;
#else

  using ControlSinkVector = std::vector<typename SettingsControlSinkTypesT<RadioSettingsT>::Variant >;
  using ControlSourceVector = std::vector<typename SettingsControlSourceTypesT<RadioSettingsT>::Variant>;
#endif


  RadioControlT()
    : m_controlSinks()
   , m_controlSources()
   , m_internalSink(this)
  {
  }
  ~RadioControlT() override = default;

  ResultCode configure(const Config::Control::Fields& config, ResolveDottedStringFunc resolver)
  {
    ResultCode rc = ResultCode::OK;
    for (auto& controllerConfig : config.sinks) {
      m_controlSinks.emplace_back();
      ResultCode rc = SettingsControlSinkFactoryT<RadioSettingsT>::create(controllerConfig, resolver, m_controlSinks.back());
      if (rc != ResultCode::OK) {
        return rc;
      }
    }

    for (auto& controllerConfig : config.sources) {
      m_controlSources.emplace_back();
      // SettingsControlSourceVariant source;
      rc = SettingsControlSourceFactoryT<RadioSettingsT>::create(controllerConfig, resolver, m_controlSources.back());
      if (rc == ResultCode::OK) {
        rc = visit([this](auto&& s) -> ResultCode {
          using T = decay_t<decltype(s)>;
          if constexpr (!is_same_v<T, monostate>) {
            s.connectRadioSettingsSink(&m_internalSink);
            s.connectFieldUpdateSink(&m_internalSink);
            return ResultCode::OK;
          } else
          {
            return ResultCode::ERR_SETTING_CONTROL_NO_SOURCES_DEFINED;
          }
        }, m_controlSources.back());
        if (rc == ResultCode::OK) {
          // m_controlSources.emplace_back(move(source));
          break;
        }
      } else {
        return rc ; //ResultCode::ERR_CONTROL_SOURCE_NOT_FOUND;
      }
    }
    return rc;
  }

  ResultCode start()
  {
    for (auto& pSink : m_controlSinks) {
      ResultCode rc = visit([&pSink](auto&& sink) -> ResultCode
      {
        using T = decay_t<decltype(sink)>;
        if constexpr (is_same_v<T, monostate>) {
          return ResultCode::ERR_SETTING_CONTROL_NO_SINKS;
        } else {
          if (sink.discover()) {
            return sink.open();
          }
        }
        return ResultCode::ERR_SETTING_CONTROL_SINK_DISCOVER;
      }, pSink);
      if (rc != ResultCode::OK) {
        return rc;
      }
    }
    // printf("[RadioControl]\t Sinks started.\r\n");
    for (auto& pSource : m_controlSources) {
      ResultCode rc = visit([&pSource](auto&& source) -> ResultCode
      {
        using T = decay_t<decltype(source)>;
        if constexpr (is_same_v<T, monostate>) {
          return ResultCode::ERR_SETTING_CONTROL_NO_SOURCES;
        } else {
          if (source.discover()) {
            return source.open();
          }
          return ResultCode::ERR_SETTING_CONTROL_SOURCE_DISCOVER;
        }
      }, pSource);
      if (rc != ResultCode::OK) {
        return rc;
      }
    }
    return ResultCode::OK;
  }

  void stop() {}

  ResultCode applySettings(RadioSettingsT& settings) override
  {
    for (auto& sinkVar : m_controlSinks) {
      ResultCode rc = visit([&settings] (auto&& sink) -> ResultCode
      {
        return sink.applySettings(settings);
      }, sinkVar);
      if (rc != ResultCode::OK) {
        return rc;
      }
    }
    return ResultCode::OK;
  }

  ResultCode applyFieldUpdate(const FieldUpdate& settingDelta) override
  {
    for (auto& sinkVar : m_controlSinks) {
      const ResultCode rc = visit([&settingDelta] (auto&& sink) -> ResultCode
      {
        return sink.applyFieldUpdate(settingDelta);
      }, sinkVar);
      if (rc != ResultCode::OK) {
        return rc;
      }
    }
    return ResultCode::OK;
  }


  void connectRadioSettingsSink(RadioSettingsSinkT<RadioSettingsT>* sink) override
  {
    m_externalSettingsSink = sink;
  }

  void connectFieldUpdateSink(FieldUpdateSink* sink) override
  {
    m_externalFieldUpdateSink = sink;
  }

  ResultCode notifySettings(RadioSettingsT& settings) override
  {
    if (m_externalSettingsSink) {
      return m_externalSettingsSink->applySettings(settings);
    }
    return ResultCode::OK;
  }
  ResultCode notifyFieldUpdate(const FieldUpdate& settingUpdate) override
  {
    if (m_externalFieldUpdateSink) {
      return m_externalFieldUpdateSink->applyFieldUpdate(settingUpdate);
    }
    return ResultCode::OK;
  }

  // PttSink Method
  ResultCode ptt(bool on) override
  {
    FieldPath path{/*makesdr_RadioSettingsPb_ptt_tag*/4}; // TODO: Red Alert! Need to deal with tags!
    FieldUpdate setting(path, on, FieldUpdateMeaning::VALUE, true);
    return applyFieldUpdate(setting);
  }


protected:
  // Intercepts settings from m_controlSources for anything relevant to this RadioControl mechanism
  // One of these gets connect()ed to each control source
  // ControlSources have no concept of BandSettings, since these are internal to the radio.
  class InternalSink : public RadioSettingsSinkT<RadioSettingsT>, public FieldUpdateSink
  {
  public:
    explicit InternalSink(RadioControlT* pControl) : m_pControl(pControl) {}
    ResultCode applySettings(RadioSettingsT& settings) override
    {
      if (m_pControl) {
        return m_pControl->notifySettings(settings); // Notify external sink
      }
      return ResultCode::OK;
    }
    ResultCode applyFieldUpdate(const FieldUpdate& settingDelta) override
    {
      if (m_pControl) {
        return m_pControl->notifyFieldUpdate(settingDelta); // Notify external sink
      }
      return ResultCode::OK;
    }

  protected:
    RadioControlT* m_pControl;
  };
protected:
  ControlSinkVector m_controlSinks;
  ControlSourceVector m_controlSources;
  InternalSink m_internalSink;
  RadioSettingsSinkT<RadioSettingsT>* m_externalSettingsSink{nullptr};
  FieldUpdateSink* m_externalFieldUpdateSink{nullptr};
};