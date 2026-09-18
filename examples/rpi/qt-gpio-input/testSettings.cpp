#include "testSettings.h"

ResultCode indirectUpdate(RadioSettings& radioSettings)
{
  bool isIndirect = false;
  AutoCompleteTrigger trigger;

  FieldPath frequencyPath;
  ResultCode rc = RadioSettings::resolveDottedPath("active_bands.focus_band.focus_pipeline.base.rf.centre_frequency", frequencyPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return rc;

  FieldUpdate frequencyUpdate(frequencyPath, static_cast<int32_t>(1), FieldUpdate::DELTA, isIndirect, trigger);
  rc = radioSettings.applyFieldUpdate(frequencyUpdate);
  if (rc != ResultCode::OK) return rc;

  FieldPath bandPath;
  rc = RadioSettings::resolveDottedPath("active_bands.focus_band.band_request", bandPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return rc;

  NameString band80 = "80m";
  FieldUpdate band80Update(bandPath, band80, FieldUpdate::VALUE, isIndirect, trigger);
  rc = radioSettings.applyFieldUpdate(band80Update);
  if (rc != ResultCode::OK) return rc;

  NameString band40 = "40m";
  FieldUpdate band40Update(bandPath, band40, FieldUpdate::VALUE, isIndirect, trigger);
  rc = radioSettings.applyFieldUpdate(band40Update);
  if (rc != ResultCode::OK) return rc;

  return radioSettings.applyFieldUpdate(band80Update);
}