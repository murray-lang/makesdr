#include "testSettings.h"

ResultCode indirectUpdate(RadioSettings& radioSettings)
{
  bool isIndirect = false;
  AutoCompleteTrigger trigger;

  SettingPath frequencyPath;
  ResultCode rc = RadioSettings::resolveDottedPath("active_bands.focus_band.focus_pipeline.base.rf.centre_frequency", frequencyPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return rc;

  SettingUpdate frequencyUpdate(frequencyPath, static_cast<int32_t>(1), SettingUpdate::DELTA, isIndirect, trigger);
  rc = radioSettings.applySettingUpdate(frequencyUpdate);
  if (rc != ResultCode::OK) return rc;

  SettingPath bandPath;
  rc = RadioSettings::resolveDottedPath("active_bands.focus_band.band_request", bandPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return rc;

  NameString band80 = "80m";
  SettingUpdate band80Update(bandPath, band80, SettingUpdate::VALUE, isIndirect, trigger);
  rc = radioSettings.applySettingUpdate(band80Update);
  if (rc != ResultCode::OK) return rc;

  NameString band40 = "40m";
  SettingUpdate band40Update(bandPath, band40, SettingUpdate::VALUE, isIndirect, trigger);
  rc = radioSettings.applySettingUpdate(band40Update);
  if (rc != ResultCode::OK) return rc;

  return radioSettings.applySettingUpdate(band80Update);
}