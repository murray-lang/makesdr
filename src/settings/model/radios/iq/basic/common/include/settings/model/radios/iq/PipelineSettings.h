#pragma once
#include <settings/model/proto/RadioSettings.pb.h>

#include <settings/model/radios/basic/WithModeT.h>

#include "PipelineRfSettings.h"
#include "IqCorrectionSettings.h"

class PipelineSettings :
  public WithModeT<
    makesdr_PipelineSettingsPb,
    makesdr_PipelineSettingsPb_mode_request_tag,
    makesdr_PipelineSettingsPb_mode_tag
    >
  , public IApplyBandDefaults
{
public:

  using Proto = makesdr_PipelineSettingsPb;

  PipelineSettings(Proto& raw)
  : WithModeT(raw)
  , m_rawSettings(raw)
  , m_rfSettings(raw.rf)
  , m_iqCorrectionSettings(raw.iq_corrections)
  {}

  [[nodiscard]] bool hasRfSettings() const { return m_rawSettings.has_rf; }
  PipelineRfSettings& rfSettings() { return m_rfSettings; }
  [[nodiscard]] const PipelineRfSettings& rfSettings() const { return m_rfSettings; }

  [[nodiscard]] bool hasIqCorrectionSettings() const { return m_rawSettings.has_iq_corrections; }
  IqCorrectionSettings& iqCorrectionSettings() { return m_iqCorrectionSettings; }
  [[nodiscard]] const IqCorrectionSettings& iqCorrectionSettings() const { return m_iqCorrectionSettings; }

  ResultCode autoComplete(const ModeList* modes)
  {
    return autoCompleteMode(modes);
  }
  ResultCode autoComplete(const FieldDescriptor& setting, uint32_t startIndex, const ModeList* modes)
  {
   const FieldPath& path = setting.getPath();
    if (startIndex >= path.size()) {
      return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
    }
    if (path[startIndex] == makesdr_PipelineSettingsPb_mode_request_tag) {
      return autoCompleteMode(setting, startIndex + 1, modes);
    }
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
  }
  ResultCode applyBandDefaults(const Band& band, const BandCategoryList* bands, const ModeList* modes) override
  {
    setModeRequest(band.defaultMode());
    ResultCode rc = autoCompleteMode(modes);
    if (rc != ResultCode::OK) return rc;
    return m_rfSettings.applyBandDefaults(band, bands, modes);
  }


protected:
  Proto& m_rawSettings;
  PipelineRfSettings m_rfSettings;
  IqCorrectionSettings m_iqCorrectionSettings;
};
