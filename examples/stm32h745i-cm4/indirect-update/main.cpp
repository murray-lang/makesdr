
#include "settings/model/core/RadioSettings.h"

#include "settings/model/proto/RadioSettings.pb.h"
#include "settings/model/meta/generalCoverageRadioMeta.h"

using RadioSettingsPb = RadioSettings_RadioSettingsPb;

#include <settings/model/proto/ProtobufIo.h>
#include "../../data/exampleRadioSettings.h"

RadioSettingsPb radioSettingsPb = RadioSettings_RadioSettingsPb_init_zero;
BandSettingsCache bandSettingsCache;


int main()
{
  RadioSettings radioSettings(exampleRadioSettingsPb, generalCoverageRadioMeta, bandSettingsCache);

  bool isIndirect = false;
  AutoCompleteTrigger trigger;

  SettingFieldPath frequencyPath;
  ResultCode rc = RadioSettings::resolveDottedPath("active_bands.focus_band.focus_pipeline.base.rf.centre_frequency", frequencyPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  SettingFieldUpdate frequencyUpdate(frequencyPath, static_cast<int32_t>(1), SettingFieldUpdate::DELTA, isIndirect, trigger);
  rc = radioSettings.applySettingFieldUpdate(frequencyUpdate);
  if (rc != ResultCode::OK) return -1;

  SettingFieldPath bandPath;
  rc = RadioSettings::resolveDottedPath("active_bands.focus_band.band_request", bandPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  NameString band80 = "80m";
  SettingFieldUpdate band80Update(bandPath, band80, SettingFieldUpdate::VALUE, isIndirect, trigger);
  rc = radioSettings.applySettingFieldUpdate(band80Update);
  if (rc != ResultCode::OK) return -1;

  NameString band40 = "40m";
  SettingFieldUpdate band40Update(bandPath, band40, SettingFieldUpdate::VALUE, isIndirect, trigger);
  rc = radioSettings.applySettingFieldUpdate(band40Update);
  if (rc != ResultCode::OK) return -1;

  rc = radioSettings.applySettingFieldUpdate(band80Update);
  if (rc != ResultCode::OK) return -1;

  return 0;
}