#include <iostream>
#include <sstream>
#include <fstream>

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

  SettingPath frequencyPath;
  ResultCode rc = RadioSettings::resolveDottedPath("active_bands.focus_band.focus_pipeline.base.rf.centre_frequency", frequencyPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  SettingUpdate frequencyUpdate(frequencyPath, 1, SettingUpdate::DELTA, isIndirect, trigger);
  rc = radioSettings.applySettingUpdate(frequencyUpdate);
  if (rc != ResultCode::OK) return -1;

  SettingPath bandPath;
  rc = RadioSettings::resolveDottedPath("active_bands.focus_band.band_request", bandPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  NameString band80 = "80m";
  SettingUpdate band80Update(bandPath, band80, SettingUpdate::VALUE, isIndirect, trigger);
  rc = radioSettings.applySettingUpdate(band80Update);
  if (rc != ResultCode::OK) return -1;

  NameString band40 = "40m";
  SettingUpdate band40Update(bandPath, band40, SettingUpdate::VALUE, isIndirect, trigger);
  rc = radioSettings.applySettingUpdate(band40Update);
  if (rc != ResultCode::OK) return -1;

  rc = radioSettings.applySettingUpdate(band80Update);
  if (rc != ResultCode::OK) return -1;

  return 0;
}