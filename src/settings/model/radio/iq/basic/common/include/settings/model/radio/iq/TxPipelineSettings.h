#pragma once

#include "PipelineSettings.h"

class TxPipelineSettings
{
public:

  using Proto = makesdr_TxPipelineSettingsPb;

  TxPipelineSettings(Proto& raw)
    : m_rawSettings(raw)
    , m_base(raw.base)
  {}

  PipelineSettings& base() { return m_base; }
  [[nodiscard]] const PipelineSettings& base() const { return m_base; }

  ResultCode autoComplete(const ModeList* modes) { return m_base.autoComplete(modes); }
  ResultCode autoComplete(const FieldDescriptor& setting, uint32_t startIndex, const ModeList* modes)
  {
    const FieldPath& path = setting.getPath();
    if (startIndex >= path.size()) {
      return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
    }
    if (path[startIndex] == makesdr_TxPipelineSettingsPb_base_tag) {
      return m_base.autoComplete(setting, startIndex + 1, modes);
    }
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
  }

private:
  Proto& m_rawSettings;
  PipelineSettings m_base;

};