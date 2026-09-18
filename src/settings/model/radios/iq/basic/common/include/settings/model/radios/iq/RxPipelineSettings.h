#pragma once

#include <ResultCode.h>
#include "PipelineSettings.h"
#include <settings/model/message/FieldPath.h>
#include <settings/model/radios/AgcSpeed.h>
#include <settings/model/message/FieldDescriptor.h>
#include <settings/model/data/mode/ModeList.h>
// #include <settings/model/IfSettings.h>

class RxPipelineSettings
{
public:

  using Proto = makesdr_RxPipelineSettingsPb;

  RxPipelineSettings(Proto& raw)
    : m_rawSettings(raw)
    , m_base(raw.base)
  {}

  PipelineSettings& base() { return m_base; }
  [[nodiscard]] const PipelineSettings& base() const { return m_base; }


  [[nodiscard]] bool hasMute() const { return m_rawSettings.has_mute; }
  [[nodiscard]] bool mute() const { return m_rawSettings.mute; }

  [[nodiscard]] bool hasAgcSpeed() const { return m_rawSettings.has_agc_speed; }
  [[nodiscard]] AgcSpeed agcSpeed() const { return static_cast<AgcSpeed>(m_rawSettings.agc_speed); }

  ResultCode autoComplete(const ModeList* modes) { return m_base.autoComplete(modes); }
  ResultCode autoComplete(const FieldDescriptor& setting, uint32_t startIndex, const ModeList* modes)
  {
    const FieldPath& path = setting.getPath();
    if (startIndex >= path.size()) {
      return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
    }
    if (path[startIndex] == makesdr_RxPipelineSettingsPb_base_tag) {
      return m_base.autoComplete(setting, startIndex + 1, modes);
    }
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
  }

private:
  Proto& m_rawSettings;
  PipelineSettings m_base;

};