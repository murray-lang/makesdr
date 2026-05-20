#include "WriteableActiveBandSettings.h"

#include "ResolvedActiveBandsFieldPaths.h"


ResultCode
WriteableActiveBandSettings::autoComplete(
  const SettingFieldPath& path,
  uint32_t startingAtIndex,
  AutoCompleteTrigger trigger
)
{
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  switch (path[startingAtIndex]) {
  case RadioSettings_ActiveBandSettingsPb_band_1_tag:
    return m_bandOneSettings.autoComplete(path, startingAtIndex + 1, trigger);
  case RadioSettings_ActiveBandSettingsPb_band_2_tag:
    return m_bandTwoSettings.autoComplete(path, startingAtIndex + 1, trigger);
  case RadioSettings_ActiveBandSettingsPb_is_split_tag:
    if (trigger == AutoCompleteTrigger::SPLIT_BAND) {
      return autoCompleteSplit();
    }

  default: break;
  }
  return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
}

ResultCode
WriteableActiveBandSettings::resolveIndirection(
  const SettingFieldPath& indirectPath,
  uint32_t startingAtIndex,
  SettingFieldPath& resolvedPath
  )
{
  if (startingAtIndex >= indirectPath.size()) {
    return ResultCode::ERR_SETTING_RESOLVE_INDIRECTION_PATH_INVALID;
  }
  switch (indirectPath[startingAtIndex]) {
  case RadioSettings_ActiveBandSettingsPb_focus_band_tag:
    {
      SplitBandId bandId = focusBandId();
      if (bandId == SplitBandId::One) {
        resolvedPath.emplace_back(RadioSettings_ActiveBandSettingsPb_band_1_tag);
        return m_bandOneSettings.resolveIndirection(indirectPath, startingAtIndex + 1, resolvedPath);
      }
      if (bandId == SplitBandId::Two) {
        resolvedPath.emplace_back(RadioSettings_ActiveBandSettingsPb_band_2_tag);
        return m_bandTwoSettings.resolveIndirection(indirectPath, startingAtIndex + 1, resolvedPath);
      }
      return ResultCode::ERR_SETTING_BAND_SETTINGS_FOCUS_BAND_NOT_SET;
    }
    break;
  default: break;
  }
  for (uint32_t i = startingAtIndex; i < indirectPath.size(); i++) {
    resolvedPath.emplace_back(indirectPath[i]);
  }
  return ResultCode::OK;
}

ResultCode
WriteableActiveBandSettings::autoCompleteSplit()
{
  SplitBandId txId = txBandId();
  if (isSplit()) {
    if (txId == SplitBandId::None) {
      if (m_rawSettings.band_2.which_band_or_request == 0) {
        m_rawSettings.tx_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
        m_rawSettings.has_tx_band_id = true;
        return ResultCode::OK;
      }
      ResultCode rc = ResultCode::OK;
      if (m_rawSettings.band_2.which_band_or_request == RadioSettings_BandSettingsPb_band_request_tag) {
        rc = m_bandTwoSettings.autoCompleteBandRequest();
      } // Otherwise m_rawSettings.band_2.which_band_or_request == RadioSettings_BandSettingsPb_band_tag
      if (rc == ResultCode::OK) {
        m_rawSettings.tx_band_id = RadioSettings_SplitBandId_SPLIT_BAND_TWO;
        m_rawSettings.has_tx_band_id = true;
      }
    }
  } else {
    if (txId != SplitBandId::One) {
      m_rawSettings.tx_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
      m_rawSettings.has_tx_band_id = true;
    }
    if (focusBandId() != SplitBandId::One) {
      m_rawSettings.focus_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
      m_rawSettings.has_focus_band_id = true;
    }
    if (rxBandId() != SplitBandId::One) {
      m_rawSettings.rx_band_id = RadioSettings_SplitBandId_SPLIT_BAND_ONE;
      m_rawSettings.has_rx_band_id = true;
    }
  }
  return ResultCode::OK;
}


// ResultCode
// WriteableActiveBandSettings::setMode(SplitBandId bandId, PipelineId pipelineId, const Mode::Type& mode)
// {
//   const SettingFieldPath* specifiedModePath = nullptr;
//   const SettingFieldPath* txPipelineIdPath = nullptr;
//   const SettingFieldPath* txPipelineModePath = nullptr;
//
//   if (bandId == SplitBandId::One) {
//     txPipelineIdPath = &band_1_tx_pipeline_id;
//     txPipelineModePath = &band_1_tx_pipeline_mode;
//     if (pipelineId == PipelineId::A) {
//       specifiedModePath = &band_1_pipeline_a_mode;
//
//     } else if (pipelineId == PipelineId::B) {
//       specifiedModePath = &band_1_pipeline_b_mode;
//     } else {
//       return ResultCode::ERR_SETTING_PIPELINE_ID_INVALID;
//     }
//   } else if (bandId == SplitBandId::Two) {
//     txPipelineIdPath = &band_2_tx_pipeline_id;
//     txPipelineModePath = &band_1_tx_pipeline_mode;
//     if (pipelineId == PipelineId::A) {
//       specifiedModePath = &band_2_pipeline_a_mode;
//     } else if (pipelineId == PipelineId::B) {
//       specifiedModePath = &band_2_pipeline_b_mode;
//     } else {
//       return ResultCode::ERR_SETTING_PIPELINE_ID_INVALID;
//     }
//   } else {
//     return ResultCode::ERR_SETTING_BAND_ID_INVALID;
//   }
//   SettingFieldUpdate specifiedUpdate(*specifiedModePath, static_cast<uint32_t>(mode));
//   ResultCode rc = m_manipulator.updateField(specifiedUpdate);
//
//   if (rc == ResultCode::OK) {
//     // Now see if the tx pipeline follows the given rx pipeline.
//     // If so then update it too.
//     SettingFieldVariant txPipelineId;
//     rc = m_manipulator.getField(*txPipelineIdPath, txPipelineId);
//     if (rc == ResultCode::OK) {
//       auto* txPipelineIdPtr = etl::get_if<uint32_t>(&txPipelineId);
//       if (txPipelineIdPtr != nullptr) {
//         if (*txPipelineIdPtr == static_cast<uint32_t>(pipelineId)) {
//           SettingFieldUpdate txUpdate(*txPipelineModePath, static_cast<uint32_t>(mode));
//           rc = m_manipulator.updateField(txUpdate);
//         }
//       } else {
//         rc = ResultCode::ERR_SETTING_PIPELINE_ID_INVALID;
//       }
//     }
//   }
//   return rc;
// }
