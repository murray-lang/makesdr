#include "../include/settings/control//radio/RadioControl.h"
#include <settings/control/factory/SettingsControlSinkFactory.h>
#include <settings/control/factory/SettingsControlSourceFactory.h>



RadioControl::RadioControl() : m_internalSink(this)
{
}

ResultCode
RadioControl::configure(const Config::Control::Fields& config)
{
  ResultCode rc = ResultCode::OK;
  for (auto& controllerConfig : config.sinks) {
    SettingsControlSinkVariant sink;
    rc = SettingsControlSinkFactory::create(controllerConfig, sink);
    if (rc == ResultCode::OK) {
      m_controlSinks.emplace_back(move(sink));
    } else {
      return rc;
    }
  }

  for (auto& controllerConfig : config.sources) {
    SettingsControlSourceVariant source;
    rc = SettingsControlSourceFactory::create(controllerConfig, source);
    if (rc == ResultCode::OK) {
      rc = visit([this](auto&& s) -> ResultCode {
        using T = decay_t<decltype(s)>;
        if constexpr (!is_same_v<T, monostate>) {
          s.connectRadioSettingsSink(m_internalSink);
          s.connectSettingUpdateSink(m_internalSink);
          return ResultCode::OK;
        } else
        {
          return ResultCode::ERR_SETTINGS_CONTROL_NO_SOURCES_DEFINED;
        }
      }, source);
      if (rc == ResultCode::OK) {
        m_controlSources.emplace_back(move(source));
        break;
      }
    } else {
      return rc ; //ResultCode::ERR_CONTROL_SOURCE_NOT_FOUND;
    }
  }
  return rc;
}

void
RadioControl::connectRadioSettingsSink(RadioSettingsSink& sink)
{
  m_externalSettingsSink.emplace(sink);
}

void
RadioControl::connectSettingUpdateSink(SettingUpdateSink& sink)
{
  m_externalFieldUpdateSink.emplace(sink);
}

ResultCode
RadioControl::notifySettings(const RadioSettings& radioSettings)
{
  if (m_externalSettingsSink) {
    return m_externalSettingsSink->get().applySettings(radioSettings);
  }
  return ResultCode::OK;
}

ResultCode
RadioControl::notifySettingUpdate(const SettingUpdate& settingDelta)
{
  if (m_externalFieldUpdateSink) {
    return m_externalFieldUpdateSink->get().applySettingUpdate(settingDelta);
  }
  return ResultCode::OK;
}

ResultCode
RadioControl::applySettings(const RadioSettings& settings)
{
  for (auto& sinkVar : m_controlSinks) {
    const ResultCode rc = visit([&settings] (auto&& sink) -> ResultCode
    {
      return sink.applySettings(settings);
    }, sinkVar);
    if (rc != ResultCode::OK) {
      return rc;
    }
  }
  return ResultCode::OK;
}

ResultCode
RadioControl::applySettingUpdate(const SettingUpdate& setting)
{
  for (auto& sinkVar : m_controlSinks) {
    const ResultCode rc = visit([&setting] (auto&& sink) -> ResultCode
    {
      return sink.applySettingUpdate(setting);
    }, sinkVar);
    if (rc != ResultCode::OK) {
      return rc;
    }
  }
  return ResultCode::OK;
}

ResultCode
RadioControl::start()
{
  for (auto& pSink : m_controlSinks) {
    ResultCode rc = visit([&pSink](auto&& sink) -> ResultCode
    {
      using T = decay_t<decltype(sink)>;
      if constexpr (is_same_v<T, monostate>) {
        return ResultCode::ERR_SETTINGS_CONTROL_NO_SINKS;
      } else {
        if (sink.discover()) {
          return sink.open();
        }
      }
      return ResultCode::ERR_SETTINGS_CONTROL_SINK_DISCOVER;
    }, pSink);
    if (rc != ResultCode::OK) {
      return rc;
    }
  }
  for (auto& pSource : m_controlSources) {
    ResultCode rc = visit([&pSource](auto&& source) -> ResultCode
    {
      using T = decay_t<decltype(source)>;
      if constexpr (is_same_v<T, monostate>) {
        return ResultCode::ERR_SETTINGS_CONTROL_NO_SOURCES;
      } else {
        if (source.discover()) {
          return source.open();
        }
        return ResultCode::ERR_SETTINGS_CONTROL_SOURCE_DISCOVER;
      }
    }, pSource);
    if (rc != ResultCode::OK) {
      return rc;
    }

  }
  return ResultCode::OK;
}

void
RadioControl::stop()
{

}

void
RadioControl::ptt(bool on)
{
  SettingPath path{makesdr_RadioSettingsPb_ptt_tag};
  SettingUpdate setting(path, on, SettingUpdate::VALUE);
  applySettingUpdate(setting);
}